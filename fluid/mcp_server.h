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

#ifndef FLUID_MCP_SERVER_H
#define FLUID_MCP_SERVER_H

// Start listening on localhost:port.  Must be called after Fl::visual() and
// before Fl::run() so that Fl::add_fd() integrates with the event loop.
void mcp_server_start(int port = 7777);

// Close all connections and the server socket.
void mcp_server_stop();

#endif // FLUID_MCP_SERVER_H
