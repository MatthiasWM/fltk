//
// MCP (Model Context Protocol) server for the Fast Light Tool Kit (FLTK) Fluid tool.
//
// Copyright 2026 by Matthias Melcher and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Implements a TCP server (default port 7777) that accepts line-delimited JSON
// requests and returns line-delimited JSON responses, giving LLM clients full
// control over the Fluid widget tree.
//
// Request format:  { "command": "...", "params": { ... }, "id": <any> }
// Response format: { "result": { ... }, "id": <any> }
//             or:  { "error": "...",   "id": <any> }
//

/*
What was created
fluid/mcp_bridge.py — the MCP server. All 8 tools map one-to-one to Fluid commands. Each tool call opens a fresh TCP connection to Fluid, sends one JSON line, reads one JSON line back, then closes. No persistent state needed.

.claude/settings.json — wires Claude Code to the bridge:


{
  "mcpServers": {
    "fluid": {
      "command": "/opt/homebrew/bin/python3.10",
      "args": ["/Users/matt/dev/fltk-1.5.matt/fluid/mcp_bridge.py"]
    }
  }
}
To use it
Build and launch Fluid — it prints [MCP] server listening on 127.0.0.1:7777
In a new Claude Code session in this repo, run /mcp to confirm the fluid server connected and its 8 tools are listed
Then just ask: "Create a login dialog with a username input, password input, and two buttons" — Claude will call get_capabilities, get_tree, then a sequence of add_widget calls, and you'll see each widget appear in Fluid in real time
Quick test from the terminal while Fluid is running:


echo '{"command":"get_capabilities","params":{},"id":1}' | nc -w 1 127.0.0.1 7777

*/

#include "mcp_server.h"

#include "json.hpp"
#include "Fluid.h"
#include "Project.h"
#include "nodes/Node.h"
#include "nodes/Widget_Node.h"
#include "nodes/Window_Node.h"
#include "nodes/factory.h"
#include "io/Project_Writer.h"

#include <FL/Fl.H>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------

static json cmd_get_capabilities(const json &p);
static json cmd_get_tree(const json &p);
static json cmd_add_widget(const json &p);
static json cmd_set_property(const json &p);
static json cmd_delete_widget(const json &p);
static json cmd_new_project(const json &p);
static json cmd_save_project(const json &p);
static json cmd_generate_code(const json &p);

using CmdFn = json (*)(const json &);

static const std::unordered_map<std::string, CmdFn> k_commands = {
  { "get_capabilities", cmd_get_capabilities },
  { "get_tree",         cmd_get_tree         },
  { "add_widget",       cmd_add_widget       },
  { "set_property",     cmd_set_property     },
  { "delete_widget",    cmd_delete_widget    },
  { "new_project",      cmd_new_project      },
  { "save_project",     cmd_save_project     },
  { "generate_code",    cmd_generate_code    },
};

// ---------------------------------------------------------------------------
// Socket plumbing
// ---------------------------------------------------------------------------

struct Client {
  int         fd;
  std::string buf; // partial-read accumulator
};

static int server_fd_ = -1;
static std::unordered_map<int, Client> clients_;

static void set_nonblocking(int fd) {
  int flags = ::fcntl(fd, F_GETFL, 0);
  ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static json dispatch(const json &req) {
  json resp;
  resp["id"] = req.contains("id") ? req["id"] : json(nullptr);

  auto it = k_commands.find(req.value("command", ""));
  if (it == k_commands.end()) {
    resp["error"] = "unknown command";
    return resp;
  }
  try {
    const json &params = req.contains("params") ? req["params"] : json::object();
    resp["result"] = it->second(params);
  } catch (const std::exception &e) {
    resp["error"] = e.what();
  }
  return resp;
}

static void client_cb(FL_SOCKET fd, void *) {
  auto it = clients_.find(fd);
  if (it == clients_.end()) return;
  Client &c = it->second;

  char tmp[4096];
  ssize_t n = ::recv(fd, tmp, sizeof(tmp) - 1, 0);
  if (n <= 0) {
    Fl::remove_fd(fd);
    ::close(fd);
    clients_.erase(fd);
    return;
  }
  c.buf.append(tmp, (size_t)n);

  // Process every complete (newline-terminated) JSON line in the buffer.
  size_t pos;
  while ((pos = c.buf.find('\n')) != std::string::npos) {
    std::string line = c.buf.substr(0, pos);
    c.buf.erase(0, pos + 1);
    if (line.empty()) continue;

    json resp;
    try {
      resp = dispatch(json::parse(line));
    } catch (const std::exception &e) {
      resp = { {"error", e.what()}, {"id", nullptr} };
    }
    std::string out = resp.dump() + "\n";
    ::send(fd, out.c_str(), out.size(), 0);
  }
}

static void accept_cb(FL_SOCKET fd, void *) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int cfd = ::accept(fd, (struct sockaddr *)&addr, &len);
  if (cfd < 0) return;
  set_nonblocking(cfd);
  clients_[cfd] = { cfd, {} };
  Fl::add_fd(cfd, FL_READ, client_cb);
  printf("[MCP] client connected\n");
}

void mcp_server_start(int port) {
  server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) {
    fprintf(stderr, "[MCP] socket() failed: %s\n", strerror(errno));
    return;
  }
  int yes = 1;
  ::setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  set_nonblocking(server_fd_);

  struct sockaddr_in addr = {};
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  addr.sin_port        = htons((uint16_t)port);

  if (::bind(server_fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
      ::listen(server_fd_, 5) < 0) {
    fprintf(stderr, "[MCP] bind/listen failed: %s\n", strerror(errno));
    ::close(server_fd_);
    server_fd_ = -1;
    return;
  }
  Fl::add_fd(server_fd_, FL_READ, accept_cb);
  printf("[MCP] server listening on 127.0.0.1:%d\n", port);
}

void mcp_server_stop() {
  if (server_fd_ >= 0) {
    Fl::remove_fd(server_fd_);
    ::close(server_fd_);
    server_fd_ = -1;
  }
  for (auto &kv : clients_) {
    Fl::remove_fd(kv.first);
    ::close(kv.first);
  }
  clients_.clear();
}

// ---------------------------------------------------------------------------
// Tree serialization helpers
// ---------------------------------------------------------------------------

static json node_to_json(Node *n) {
  json j;
  j["uid"]  = n->get_uid();
  j["type"] = n->type_name();
  j["name"] = n->name()  ? n->name()  : "";
  j["label"]= n->label() ? n->label() : "";
  if (n->callback() && n->callback()[0])
    j["callback"] = n->callback();

  if (n->is_widget()) {
    Widget_Node *w = static_cast<Widget_Node *>(n);
    if (w->o) {
      j["x"] = w->o->x();
      j["y"] = w->o->y();
      j["w"] = w->o->w();
      j["h"] = w->o->h();
    }
  }

  json children = json::array();
  for (Node *c = n->first_child(); c; c = c->next_sibling())
    children.push_back(node_to_json(c));
  if (!children.empty())
    j["children"] = std::move(children);

  return j;
}

// ---------------------------------------------------------------------------
// Command implementations
// ---------------------------------------------------------------------------

static json cmd_get_capabilities(const json &) {
  return {
    { "commands", {
      "get_capabilities", "get_tree", "add_widget", "set_property",
      "delete_widget", "new_project", "save_project", "generate_code"
    }},
    { "protocol", "line-delimited JSON over TCP" },
    { "widget_types", json::array({
      // --- containers ---
      { {"type","Fl_Window"},     {"can_have_children",true},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Group"},      {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Flex"},       {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Tabs"},       {"can_have_children",true},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Scroll"},     {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Pack"},       {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Tile"},       {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Wizard"},     {"can_have_children",true},  {"properties",{"name","label","x","y","w","h"}} },
      // --- buttons ---
      { {"type","Fl_Button"},        {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Check_Button"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Round_Button"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Light_Button"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Return_Button"}, {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Repeat_Button"}, {"properties",{"name","label","x","y","w","h","callback"}} },
      // --- text input / output ---
      { {"type","Fl_Input"},       {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Output"},      {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_File_Input"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Text_Editor"}, {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Text_Display"},{"properties",{"name","label","x","y","w","h"}} },
      // --- valuators ---
      { {"type","Fl_Slider"},       {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Value_Slider"}, {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Scrollbar"},    {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Spinner"},      {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Counter"},      {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Adjuster"},     {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Dial"},         {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Roller"},       {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Value_Input"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Value_Output"}, {"properties",{"name","label","x","y","w","h"}} },
      // --- menus ---
      { {"type","Fl_Menu_Bar"},    {"properties",{"name","x","y","w","h"}} },
      { {"type","Fl_Menu_Button"}, {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Choice"},      {"properties",{"name","label","x","y","w","h","callback"}} },
      // --- browsers ---
      { {"type","Fl_Browser"},       {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Check_Browser"}, {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_File_Browser"},  {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Tree"},          {"properties",{"name","label","x","y","w","h","callback"}} },
      { {"type","Fl_Help_View"},     {"properties",{"name","label","x","y","w","h"}} },
      // --- misc ---
      { {"type","Fl_Box"},      {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Clock"},    {"properties",{"name","label","x","y","w","h"}} },
      { {"type","Fl_Progress"}, {"properties",{"name","label","x","y","w","h"}} },
    })},
    // Non-widget nodes the LLM may encounter in get_tree output
    { "non_widget_types", {
      "Function", "Code", "CodeBlock", "Decl", "DeclBlock", "Class", "Widget_Class", "Comment"
    }}
  };
}

static json cmd_get_tree(const json &) {
  json roots = json::array();
  for (Node *n = Fluid.proj.tree.first; n; n = n->next) {
    if (n->parent == nullptr)
      roots.push_back(node_to_json(n));
  }
  const char *fname = Fluid.proj.proj_filename;
  return {
    { "project_file", fname ? fname : "" },
    { "modified",     Fluid.proj.modflag != 0 },
    { "nodes",        std::move(roots) }
  };
}

static json cmd_add_widget(const json &p) {
  std::string type_name = p.at("type").get<std::string>();

  // Resolve parent by uid (null/absent means top-level).
  Node *parent = nullptr;
  if (p.contains("parent_uid") && !p["parent_uid"].is_null()) {
    unsigned short uid = (unsigned short)p["parent_uid"].get<int>();
    parent = Fluid.proj.tree.find_by_uid(uid);
    if (!parent)
      throw std::runtime_error("parent_uid not found");
    if (!parent->can_have_children())
      throw std::runtime_error("parent cannot have children");
  }

  // Point tree.current at the intended parent so the factory places the new
  // node as its last child.
  Node *saved_current = Fluid.proj.tree.current;
  Fluid.proj.tree.current = parent;
  Node *n = add_new_widget_from_user(type_name.c_str(),
                                     Strategy::AS_LAST_CHILD, false);
  Fluid.proj.tree.current = saved_current;

  if (!n)
    throw std::runtime_error("unknown widget type: " + type_name);

  // Apply optional common properties.
  if (p.contains("name") && p["name"].is_string())
    n->name(p["name"].get<std::string>().c_str());
  if (p.contains("label") && p["label"].is_string())
    n->label(p["label"].get<std::string>().c_str());
  if (p.contains("callback") && p["callback"].is_string())
    n->callback(p["callback"].get<std::string>().c_str());

  // Apply geometry if the node is a real widget.
  if (n->is_widget()) {
    Widget_Node *w = static_cast<Widget_Node *>(n);
    if (w->o) {
      int x = p.value("x", w->o->x());
      int y = p.value("y", w->o->y());
      int ww = p.value("w", w->o->w());
      int h  = p.value("h", w->o->h());
      w->o->resize(x, y, ww, h);
      w->o->redraw();
    }
  }

  return { {"uid", n->get_uid()}, {"type", n->type_name()} };
}

static json cmd_set_property(const json &p) {
  unsigned short uid = (unsigned short)p.at("uid").get<int>();
  Node *n = Fluid.proj.tree.find_by_uid(uid);
  if (!n) throw std::runtime_error("uid not found");

  std::string prop  = p.at("property").get<std::string>();
  const json &value = p.at("value");

  if      (prop == "name")     { n->name(value.get<std::string>().c_str()); }
  else if (prop == "label")    { n->label(value.get<std::string>().c_str()); }
  else if (prop == "callback") { n->callback(value.get<std::string>().c_str()); }
  else if (n->is_widget()) {
    Widget_Node *w = static_cast<Widget_Node *>(n);
    if (!w->o) throw std::runtime_error("widget has no live Fl_Widget");
    int x = w->o->x(), y = w->o->y(), ww = w->o->w(), h = w->o->h();
    if      (prop == "x") x  = value.get<int>();
    else if (prop == "y") y  = value.get<int>();
    else if (prop == "w") ww = value.get<int>();
    else if (prop == "h") h  = value.get<int>();
    else throw std::runtime_error("unknown property: " + prop);
    w->o->resize(x, y, ww, h);
    w->o->redraw();
  } else {
    throw std::runtime_error("unknown property: " + prop);
  }

  Fluid.proj.set_modflag(1);
  return { {"success", true} };
}

static json cmd_delete_widget(const json &p) {
  unsigned short uid = (unsigned short)p.at("uid").get<int>();
  Node *target = Fluid.proj.tree.find_by_uid(uid);
  if (!target) throw std::runtime_error("uid not found");

  // Deselect everything, then select only the target.
  for (Node *n = Fluid.proj.tree.first; n; n = n->next)
    n->new_selected = 0;
  target->new_selected = 1;
  selection_changed(target);

  Fluid.delete_selected();
  return { {"success", true} };
}

static json cmd_new_project(const json &) {
  Fluid.new_project(false);
  return { {"success", true} };
}

static json cmd_save_project(const json &p) {
  const char *fname = Fluid.proj.proj_filename;
  if (p.contains("filename") && p["filename"].is_string()) {
    // Allow the LLM to provide an explicit path.
    std::string path = p["filename"].get<std::string>();
    Fluid.proj.set_filename(path);
    fname = Fluid.proj.proj_filename;
  }
  if (!fname || !fname[0])
    throw std::runtime_error("no project filename set; provide \"filename\" in params");

  int r = fld::io::write_file(Fluid.proj, fname, 0);
  if (r == 0) throw std::runtime_error("write_file failed");
  Fluid.proj.set_modflag(0);
  return { {"success", true}, {"saved_to", fname} };
}

static json cmd_generate_code(const json &) {
  int r = Fluid.write_code_files(true);
  return {
    { "success",      r == 0 },
    { "code_file",    Fluid.proj.codefile_path() + Fluid.proj.codefile_name() },
    { "header_file",  Fluid.proj.headerfile_path() + Fluid.proj.headerfile_name() }
  };
}
