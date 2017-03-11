using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using System.Net;
using System.Net.Sockets;
using System.Net.WebSockets;
using System.Threading;

namespace MeTwo.Server
{
    public class Program
    {
        public static Program Singleton { get; } = new Program();

        private TcpClient tcp;
        private TcpListener listener;
        private WebSocket ws;

        private Program()
        {
        }

        private void Listen(IPAddress ip, int port)
        {
            listener = new TcpListener(ip, port);
            listener.Start();

            Task.Run(async () =>
            {
                while (true)
                {
                    var client = await listener.AcceptTcpClientAsync();

                    tcp?.Dispose();
                    tcp = client;
                    Console.WriteLine("Client Connected");
                }
            });
        }

        public async Task HandleWebSocket(WebSocket socket)
        {
            ws?.Dispose();
            ws = socket;

            var seg = new ArraySegment<byte>(new byte[1]);
            while (ws.State == WebSocketState.Open)
            {
                await ws.ReceiveAsync(seg, CancellationToken.None);
                if (tcp != null)
                {
                    await tcp.GetStream().WriteAsync(seg.Array, 0, seg.Count);
                }
            }
            ws.Dispose();
            ws = null;
        }

        public static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: dotnet run ip port");
                return;
            }

            if (!IPAddress.TryParse(args[0], out IPAddress ip))
            {
                Console.WriteLine("Invalid IP");
                return;
            }
            if (!int.TryParse(args[1], out int port))
            {
                Console.WriteLine("Invalid port");
                return;
            }

            Singleton.Listen(ip, port);

            var host = new WebHostBuilder()
                .UseKestrel()
                .UseUrls($"http://{ip}:80")
                .UseContentRoot(Directory.GetParent(Directory.GetCurrentDirectory()).ToString())
                .UseStartup<Startup>()
                .Build();

            host.Run();
        }
    }
}
