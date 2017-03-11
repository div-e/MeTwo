using Microsoft.AspNetCore.Builder;

namespace MeTwo.Server
{
    public class Startup
    {
        public void Configure(IApplicationBuilder app)
        {
            app.UseWebSockets();

            app.Use(async (context, next) =>
            {
                if (context.WebSockets.IsWebSocketRequest)
                {
                    await Program.Singleton.HandleWebSocket(await context.WebSockets.AcceptWebSocketAsync());
                }
                else
                {
                    await next();
                }
            });

            app.UseDefaultFiles();
            app.UseStaticFiles();
        }
    }
}
