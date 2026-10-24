using System.Text;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.Extensions.Caching.Memory;
using Microsoft.IdentityModel.Tokens;
using Models.Auth;
using Models.Proxy.Telnet;

namespace Microsoft.Extensions.DependencyInjection
{
    public static class AuthenticationConfiguration
    {
        public static IServiceCollection AddJwtAuthentication(this IServiceCollection services, IConfiguration configuration)
        {
            // 1. Extract and bind JWT settings from appsettings.json
            var jwtSettings = configuration.GetSection("JWTSettings").Get<JWTSettings>();
            if (jwtSettings == null || string.IsNullOrWhiteSpace(jwtSettings.Secret))
            {
                throw new InvalidOperationException("JWT Secret Key is missing from configuration.");
            }

            services.Configure<JWTSettings>(configuration.GetSection("JWTSettings"));
            var key = Encoding.UTF8.GetBytes(jwtSettings.Secret);

            services.AddAuthentication(options =>
            {
                options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
                options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
            })
            .AddJwtBearer(options =>
            {
                options.TokenValidationParameters = new TokenValidationParameters
                {
                    ValidateIssuerSigningKey = true,
                    IssuerSigningKey = new SymmetricSecurityKey(key),
                    ValidateIssuer = false, 
                    ValidateAudience = false,
                    ValidateLifetime = true,
                    ClockSkew = TimeSpan.Zero
                };

                options.Events = new JwtBearerEvents
                {
                    OnTokenValidated = async context =>
                    {
                        var cache = context.HttpContext.RequestServices.GetRequiredService<IMemoryCache>();

                        string rawToken = null;
                        string authHeader = context.HttpContext.Request.Headers.Authorization.ToString();
                        
                        bool hasBearerToken = (
                            !string.IsNullOrEmpty(authHeader) && 
                            authHeader.StartsWith("Bearer ", StringComparison.OrdinalIgnoreCase)
                        );

                        if (hasBearerToken)
                        {
                            rawToken = authHeader.Replace("Bearer ", "", StringComparison.OrdinalIgnoreCase).Trim();
                        }

                        string outToken = rawToken ?? "Empty";
                        Console.WriteLine($"Token:\n{outToken}");
                        if (string.IsNullOrEmpty(rawToken) || !cache.TryGetValue(rawToken, out _))
                        {
                            context.Fail("This token is invalid.");
                        }

                        await Task.CompletedTask;
                    }
                };
            });

            return services;
        }
    }
}
