import http.server as SimpleHTTPServer
import socketserver as SocketServer

PORT = 8080

class GetHandler(
        SimpleHTTPServer.SimpleHTTPRequestHandler
        ):
    def end_headers (self):
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')  
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        SimpleHTTPServer.SimpleHTTPRequestHandler.end_headers(self)

Handler = GetHandler
httpd = SocketServer.TCPServer(("", PORT), Handler)

httpd.serve_forever()
