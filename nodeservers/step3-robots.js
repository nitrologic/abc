var express=require('express')
var http=require('http')
var fs=require('fs')
var app=express()

var log=fs.WriteStream('log.txt',{'flags':'a'});


app.use('/robots.txt', function(req, res){
  log.write('robot.txt:'+req.headers['user-agent'] + '\n');
  res.send('');
});

app.use(function(req, res, next) {
  log.write('404:'+req.url+'\n');
  log.write('agent:'+req.headers['user-agent']+'\n');
  log.write('address:'+req.connection.remoteAddress+'\n');
  res.status(404).send('404: Sorry '+req.url+' Not Found '+req.connection.remoteAddress);
});

http.createServer(app).listen(80)
console.log("upload server listening on port 80\n")
log.write("server::start\n");

