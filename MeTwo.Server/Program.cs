using System;
using System.IO;
using System.Linq;
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

        public IPAddress LocalIP { get; private set; }
        public int WebPort { get; } = 80;
        public int TcpPort { get; } = 5000;
        private TcpClient tcp;
        private TcpListener listener;
        private WebSocket ws;

        private Program()
        {
        }

        public void Init()
        {
            var ips = Dns.GetHostAddressesAsync(Dns.GetHostName());
            ips.Wait();
            LocalIP = ips.Result.First(ip => ip.AddressFamily == AddressFamily.InterNetwork);

            Listen();
        }

        private void Listen()
        {
            listener = new TcpListener(LocalIP, TcpPort);
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
            try
            {
                Singleton.Init();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return;
            }

            var host = new WebHostBuilder()
                .UseKestrel()
                .UseUrls($"http://{Singleton.LocalIP}:{Singleton.WebPort}")
                .UseContentRoot(Directory.GetCurrentDirectory())
                .UseStartup<Startup>()
                .Build();

            host.Run();
        }
    }
}
