var http = require('http');
var fs = require('fs');

/* Chargement du fichier main.html affiché au client */
var server = http.createServer(function(req, res) { //creation du serveur en local
    fs.readFile('./Pconnexion.html', 'utf-8', function(error, content) {
        res.writeHead(200, {"Content-Type": "text/html"}); //on fait en sorte que le serveur comprenne le html
        res.end(content);
    });
});

/* Chargement de socket.io, qui permet la communication en temps réel par "messages"*/
var io = require('socket.io').listen(server);

/* Begin of synchronous listening of server */
io.sockets.on('connection', function (socket) {

    socket.emit('message', 'Vous êtes bien connecté ! '); //on verifie directement sur le site si le serveur s'est bien lancé

    socket.on('executeHelloC', function (message) { //socket de test pour voir s'il était bien possible d'appeler des fonctions en c avec du js
      console.log(message);

      fs.writeFileSync("testWrite.txt", message+";" , "UTF-8");

      var exec = require('child_process').exec;

      var cmd = 'Hello.exe';
      //var path = '';
      var child = exec(cmd, function(error, stdout, stderr) {
        if (error === null) {
            console.log(stdout);
            console.log('success');
        } else {
            console.log('error');
        }
      });
    });

    socket.on('executeconnect', function (message) { //on execute ce que le côté client nous a demandé
        console.log(message);
        fs.writeFileSync("connect.txt", message , "UTF-8"); //ecriture dans un .txt qu'on envoie pour le code c
        var exec = require('child_process').exec;
        var cmd = 'GestionClient.exe'; 
        //var path = '';
        var child = exec(cmd, function(error, stdout, stderr) {//appel de la fonction en c
          if (error === null) {
              console.log(stdout); //verification si le code s'est lancé ou non
              console.log('success');
          } else {
              console.log('error');
          }
        });
      });

});

server.listen(8080); //on positionne le serveur en localhost à la position 8080
