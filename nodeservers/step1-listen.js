var express=require('express')
var http=require('http')

var app=express()

http.createServer(app).listen(80)

console.log("upload server listening on port 80\n")

