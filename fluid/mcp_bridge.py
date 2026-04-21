#!/opt/homebrew/bin/python3.10
"""
MCP bridge for the Fluid FLTK UI designer.

Speaks MCP JSON-RPC over stdio (what Claude Code expects) and forwards
every tool call to Fluid's TCP server on localhost:7777.

Requirements:
    pip install mcp

Usage — Claude Code project settings (.claude/settings.json):
    {
      "mcpServers": {
        "fluid": {
          "command": "python3",
          "args": ["/absolute/path/to/fluid/mcp_bridge.py"]
        }
      }
    }

Usage — test from the terminal while Fluid is running:
    echo '{"command":"get_tree","params":{},"id":1}' | nc -w 1 127.0.0.1 7777

The bridge starts Fluid's TCP connection fresh for each tool call so no
persistent connection state is required on either end.
"""

import argparse
import json
import socket
from typing import Optional, Union

from mcp.server.fastmcp import FastMCP

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

FLUID_HOST = "127.0.0.1"
FLUID_PORT = 7777

# ---------------------------------------------------------------------------
# MCP server
# ---------------------------------------------------------------------------

mcp = FastMCP(
    "fluid",
    instructions=(
        "Tools to control the Fluid FLTK UI designer in real time. "
        "Start every session by calling get_capabilities() to learn which widget "
        "types exist, then get_tree() to inspect the current project state. "
        "Use the UIDs returned by get_tree() and add_widget() when referencing "
        "existing nodes. Widgets must be placed inside a container: create an "
        "Fl_Window first, then add child widgets using its UID as parent_uid. "
        "After building the UI call generate_code() to produce the .cxx/.h files."
    ),
)

# ---------------------------------------------------------------------------
# Transport helper
# ---------------------------------------------------------------------------

def call_fluid(command: str, params: Optional[dict] = None) -> dict:
    """Send one JSON command to Fluid and return the result dict."""
    req = {"command": command, "params": params or {}, "id": 1}
    payload = json.dumps(req) + "\n"
    try:
        with socket.create_connection((FLUID_HOST, FLUID_PORT), timeout=10) as s:
            s.sendall(payload.encode())
            line = s.makefile().readline()
    except ConnectionRefusedError:
        raise RuntimeError(
            f"Cannot connect to Fluid on {FLUID_HOST}:{FLUID_PORT}. "
            "Make sure Fluid is running (it prints '[MCP] server listening …' on startup)."
        )
    except socket.timeout:
        raise RuntimeError("Timed out waiting for Fluid to respond.")

    resp = json.loads(line)
    if "error" in resp:
        raise RuntimeError(f"Fluid error: {resp['error']}")
    return resp.get("result", {})

# ---------------------------------------------------------------------------
# Tools
# ---------------------------------------------------------------------------

@mcp.tool()
def get_capabilities() -> dict:
    """
    Return all commands supported by Fluid and the full catalog of widget
    types with their accepted properties.

    Call this once at the start of a session so you know which widget type
    names are valid inputs to add_widget().
    """
    return call_fluid("get_capabilities")


@mcp.tool()
def get_tree() -> dict:
    """
    Return the current Fluid project as a nested JSON tree.

    Each node contains:
      uid      — unique integer ID; use this in all other calls
      type     — C++ class name, e.g. "Fl_Window", "Fl_Button"
      name     — C++ variable name (may be empty)
      label    — visible label text (may be empty)
      x, y, w, h — geometry in pixels (widget nodes only)
      callback — callback function name (omitted if not set)
      children — array of child nodes (omitted if none)

    The top-level "nodes" array holds root-level items (usually Fl_Windows
    and Function nodes).
    """
    return call_fluid("get_tree")


@mcp.tool()
def add_widget(
    type: str,
    parent_uid: Optional[int] = None,
    name: str = "",
    label: str = "",
    x: int = 0,
    y: int = 0,
    w: int = 0,
    h: int = 0,
    callback: str = "",
) -> dict:
    """
    Add a new widget to the Fluid project.

    Args:
        type:       Widget class name, e.g. "Fl_Window", "Fl_Button", "Fl_Input".
                    Call get_capabilities() for the complete list.
        parent_uid: UID of the parent container (Fl_Window, Fl_Group, Fl_Flex, …).
                    Omit or pass null to create a top-level node such as Fl_Window.
        name:       C++ variable name for the widget (leave empty for anonymous).
        label:      Visible label text.
        x, y:       Position in pixels relative to the parent window.
                    Pass 0 to let Fluid choose a default position.
        w, h:       Size in pixels. Pass 0 to use Fluid's type default.
        callback:   C++ callback function name (no parentheses).

    Returns:
        {"uid": <int>, "type": <str>} identifying the new node.
    """
    params: dict = {"type": type}
    if parent_uid is not None:
        params["parent_uid"] = parent_uid
    if name:
        params["name"] = name
    if label:
        params["label"] = label
    if x:
        params["x"] = x
    if y:
        params["y"] = y
    if w:
        params["w"] = w
    if h:
        params["h"] = h
    if callback:
        params["callback"] = callback
    return call_fluid("add_widget", params)


@mcp.tool()
def set_property(uid: int, property: str, value: str) -> dict:
    """
    Change a single property on an existing node.

    Supported properties:
      name     — C++ variable name (string)
      label    — Visible label text (string)
      callback — Callback function name (string)
      x        — Left edge in pixels (numeric string, e.g. "10")
      y        — Top edge in pixels
      w        — Width in pixels
      h        — Height in pixels

    Args:
        uid:      Node UID from get_tree() or add_widget().
        property: One of the names listed above.
        value:    New value as a string; x/y/w/h are converted to int automatically.
    """
    parsed: Union[str, int] = value
    if property in ("x", "y", "w", "h"):
        try:
            parsed = int(value)
        except ValueError:
            pass
    return call_fluid("set_property", {"uid": uid, "property": property, "value": parsed})


@mcp.tool()
def delete_widget(uid: int) -> dict:
    """
    Delete a node and all of its children from the project.

    Args:
        uid: Node UID from get_tree() or add_widget().
    """
    return call_fluid("delete_widget", {"uid": uid})


@mcp.tool()
def new_project() -> dict:
    """
    Clear the current Fluid project and start a new empty one.
    Unsaved changes are discarded without a confirmation dialog.
    """
    return call_fluid("new_project")


@mcp.tool()
def save_project(filename: str = "") -> dict:
    """
    Save the current project to its .fl file.

    Args:
        filename: Absolute path to write. If omitted, saves to the path
                  already recorded in the project. Raises an error if no
                  path is set in either place.

    Returns:
        {"success": true, "saved_to": "<path>"}
    """
    params: dict = {}
    if filename:
        params["filename"] = filename
    return call_fluid("save_project", params)


@mcp.tool()
def generate_code() -> dict:
    """
    Generate the C++ source (.cxx) and header (.h) files from the current
    project.

    Returns:
        {"success": bool, "code_file": "<path>", "header_file": "<path>"}
    """
    return call_fluid("generate_code")


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="MCP bridge for Fluid")
    parser.add_argument(
        "--port", type=int, default=7777,
        help="TCP port Fluid is listening on (default: 7777)"
    )
    parser.add_argument(
        "--host", default="127.0.0.1",
        help="Host Fluid is listening on (default: 127.0.0.1)"
    )
    args = parser.parse_args()
    FLUID_HOST = args.host
    FLUID_PORT = args.port
    mcp.run()
