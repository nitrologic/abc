var express=require('express')
var http=require('http')
var fs=require('fs')
var app=express()

var homePage="index.html";

var log=fs.WriteStream('log.txt',{'flags':'a'});

app.get('/', function(req, res){
  res.writeHead(301,{Location: homePage});
  res.end();
});

app.use('/upload',function(req,res){
	res.send('THANKS');
	log.write('incoming:'+req.url+'\n');
});

app.use('/robots.txt', function(req, res){
  log.write('robot.txt:'+req.headers['user-agent'] + '\n');
  res.send('');
});

app.use(express.static(__dirname+'/site'));

app.use(function(req, res, next) {
  log.write('404:'+req.url+'\n');
  log.write('agent:'+req.headers['user-agent']+'\n');
  log.write('address:'+req.connection.remoteAddress+'\n');
  res.status(404).send('404: Sorry '+req.url+' Not Found '+req.connection.remoteAddress);
});

http.createServer(app).listen(8080)
console.log("upload server listening on port 8080\n")
log.write("server::start\n");

