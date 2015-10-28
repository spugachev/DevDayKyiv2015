var express = require("express");
var app = express();

var port = process.env.PORT || 3001;

app.use("/", express.static(__dirname + "/public"));

var server = app.listen(port, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log("HTTP/1.1 app listening at http://%s:%s", host, port);
});