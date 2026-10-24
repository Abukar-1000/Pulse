using Microsoft.AspNetCore.Mvc;
using Models.Requests;
using Microsoft.AspNetCore.Authorization;


namespace Controllers.Auth
{
    [Authorize]
    [ApiController]
    [Route("api/v1/[controller]")]
    public class TelnetController: ControllerBase
    {
        public TelnetController()
        {}

        [HttpPost("percise")]
        public IActionResult Authenticate([FromBody] Percise request)
        {

            return Ok(request);
        }
    }
}