var spdy = require("spdy");
var fs = require("fs");
var express = require("express");
var app = express();

var port = process.env.PORT || 3002;

var options = {
  key: fs.readFileSync(__dirname + "/localhost.key"),
  cert: fs.readFileSync(__dirname + "/localhost.crt"),

  spdy: {
    protocols: [ "h2"],
    plain: false
  }
};

app.use("/", express.static(__dirname + "/public"));

app.use(function(req, res) {
	
	//---------------------------------
	if(req.url == "/push"){
		for(var i=0; i<10; i++){
			var stream = res.push("/data" + i + ".js", 
			{
				"content-type": "application/javascript"
			});

			stream.end("alert('Stream SUCCESSFUL: "+ i +"!');");
		}
	}
	//---------------------------------
  
	var fstream = fs.createReadStream(__dirname + "/public/index.html");
	fstream.pipe(res);
});

var server = spdy.createServer(options, app);
server.listen(port);