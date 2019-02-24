'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

app.get('/getFileList', function(req , res){
  
  const testFolder = './uploads/';
  var array = [];

  fs.readdirSync(testFolder).forEach(file => {
   
    var pos = -1;
    pos = file.search(".ged");

    if(pos != -1) {
      
      array.push(file);
    }
    
  });

  res.send(array);

});



app.post('/createFile', function(req , res){
  
  const testFolder = './uploads/';

  //console.log(req.body);

  var fname = testFolder + req.body.fileName;

  var fileStr = "0 HEAD\n";
  fileStr += "1 SOUR " + req.body.source + "\n";
  fileStr += "1 GEDC\n2 VERS " + req.body.gedVers + "\n";
  fileStr += "1 CHAR " + req.body.encoding + "\n";
  fileStr += "1 SUBM @SUB1@\n";

  fileStr += "0 @SUB1@ SUBM\n";
  fileStr += "1 NAME " + req.body.subName + "\n";

  if(req.body.subAddr != '') {

    fileStr += "1 ADDR " + req.body.subAddr + "\n";
  }

  fileStr += "0 TRLR\n";

  
  fs.writeFile(fname, fileStr, function(err) {

    if(err) {
        return console.log(err);
    }
  //console.log("The file was saved!");
  });

});


let sharedLib = ffi.Library('./sharedLib', {

  'createFileSum': [ 'string', ['string'] ],
  'populateGEDview': [ 'string', ['string'] ],
  'addJSONind': [ 'string', ['string', 'string'] ],
  'getJSONdesc': [ 'string', ['string', 'string', 'int'] ],
  'getJSONancs': [ 'string', ['string', 'string', 'int'] ],

});


app.post('/makeSummary', function(req , res){
  
  //console.log('body: ' + req.body.filename);
  let fileSum = sharedLib.createFileSum("./uploads/" + req.body.filename);
  var str = JSON.stringify(fileSum).replace(/\\n|\\r/g, '');
  //console.log("Testing: " + str);

  var final = JSON.parse(str);

  res.send(final);

});

app.post('/makeView', function(req , res){
  
  //console.log('body: ' + req.body.filename);
  let viewData = sharedLib.populateGEDview("./uploads/" + req.body.filename);
  var str = JSON.stringify(viewData).replace(/\\n|\\r/g, '');
  //console.log("Testing: " + str);

  var final = JSON.parse(str);

  res.send(final);

});

app.post('/addIndi', function(req , res){

  var filename = "./uploads/" + req.body.filename;
  delete req.body["filename"];
  //console.log(filename);

  var string = JSON.stringify(req.body);
  //console.log("T: " + string);

  let rtrnVal = sharedLib.addJSONind(string, filename);

  res.send(rtrnVal);

});

app.post('/getDesc', function(req , res){

  var filename = "./uploads/" + req.body.filename;
  delete req.body["filename"];

  var genNum = parseInt(req.body.genNum); 
  delete req.body["genNum"];

  var string = JSON.stringify(req.body).replace(/\s/g, '');

  let descList = sharedLib.getJSONdesc(filename, string, genNum);

  //console.log("T: " + decsList);

  res.send(descList);

});

app.post('/getAncs', function(req , res){

  var filename = "./uploads/" + req.body.filename;
  delete req.body["filename"];

  var genNum = parseInt(req.body.genNum); 
  delete req.body["genNum"];

  var string = JSON.stringify(req.body).replace(/\s/g, '');

  let ancsList = sharedLib.getJSONancs(filename, string, genNum);

  //console.log("T: " + decsList);

  res.send(ancsList);

});

//////// Assignment 4 ////////////

const mysql = require('mysql');
var connection;

app.post('/conDB', function(req , res){

  connection = mysql.createConnection({

    host: 'dursley.socs.uoguelph.ca',
    user: req.body.username,
    password: req.body.password,
    database: req.body.database

  });

  connection.connect(function(err){
  if(!err) {
      
      console.log("Database is connected ...");
      res.send("OK");

  } else {

      console.log("Error connecting database ...");
      res.send("FAIL");
  }
  });


});



app.get('/storeFiles', function(req , res){

  connection.query("CREATE TABLE FILE (file_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, file_Name VARCHAR(60) NOT NULL, source VARCHAR(250) NOT NULL, version VARCHAR(10) NOT NULL, encoding VARCHAR(10) NOT NULL, sub_name VARCHAR(62) NOT NULL, sub_addr VARCHAR(256), num_individuals INT, num_families INT )", function (err, rows, fields) {
      
      if (err) console.log("ALERT: " + err);

  });

  connection.query("CREATE TABLE INDIVIDUAL (ind_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, surname VARCHAR(256) NOT NULL, given_name VARCHAR(256) NOT NULL, sex VARCHAR(1), fam_size INT, source_file INT, FOREIGN KEY(source_file) REFERENCES FILE(file_id) ON DELETE CASCADE )", function (err, rows, fields) {
      
      if (err) console.log("ALERT: " + err);

  });

  const testFolder = './uploads/';
  var array = [];

  fs.readdirSync(testFolder).forEach(file => {
   
    var pos = -1;
    pos = file.search(".ged");

    if(pos != -1) {
      
      array.push(file);
    }
    
  });

  insertFiles(array);

  res.send("OK");

});

function insertFiles(array) {

  for(let file of array) {

    let fileSum = sharedLib.createFileSum("./uploads/" + file);

    var str = JSON.stringify(fileSum).replace(/\\n|\\r/g, '');
    var last = JSON.parse(str);
    var final = JSON.parse(last);

    let records = "INSERT INTO FILE (file_Name, source, version, encoding, sub_name, sub_addr, num_individuals, num_families) VALUES (\"" + file + "\", \"" + final.source + "\", \"" + final.GEDCvers + "\", \"" + final.encoding + "\", \"" + final.subName + "\", \"" + final.subAddr + "\", \"" + final.numInd + "\", \"" + final.numFam + "\")";
    
    connection.query(records, function (err, result) {
        
        if (err) {
            
          console.log("ALERT: " + err);

        }else {

          insertIndividuals(file, result.insertId);
        }
    });

  }

}

function insertIndividuals(file, file_id) {

  let viewData = sharedLib.populateGEDview("./uploads/" + file);
  
  if(viewData != "") {
  
    var str = JSON.stringify(viewData).replace(/\\n|\\r/g, '');

    var final = JSON.parse(str);
    //console.log("Testing: " + last);
     
    insertLoop(final, file_id);
  }
  
}

function insertLoop(final, file_id) {

  var last = JSON.parse(final);

  for(let ind of last) {

    var records = "INSERT INTO INDIVIDUAL (surname, given_name, sex, fam_size, source_file) VALUES (\"" + ind.surname + "\", \"" + ind.givenName + "\", \"" + ind.gender + "\", \"" + ind.famSize + "\", \"" + file_id + "\")";
    
    connection.query(records, function (err, rows, fields) {
      if (err) {
        
        console.log("ALERT: " + err);
      }

    });
  }
   
}


app.get('/clearData', function(req , res){

  connection.query("DELETE FROM FILE", function (err, rows, fields) {
    
    if (err) {
        console.log("ALERT: " + err);
    } else {
       // console.log("Cleared data from FILE");
        res.send("OK");
        
    }

  });

});

app.get('/fileCount', function(req , res){

  var numFiles;
 
  var nFiles = function(callback) {

    connection.query("SELECT COUNT(*) AS total FROM FILE", function(err,rows) {
     
      numFiles = rows[0].total;
      //console.log("TF: " + numFiles);
      callback(null, numFiles);
     
    });

  }

  nFiles(function (err, numFiles) {

    //console.log("T: " + numFiles);
    var str = JSON.stringify(numFiles);
    res.send(str);
    
  });

});

app.get('/indCount', function(req , res){

  var numInds;
 
  var nInds = function(callback) {

    connection.query("SELECT COUNT(*) AS total FROM INDIVIDUAL", function(err,rows) {
     
      numInds = rows[0].total;
      //console.log("TF: " + numFiles);
      callback(null, numInds);
     
    });

  }

  nInds(function (err, numInds) {

    //console.log("T: " + numInds);
    var str = JSON.stringify(numInds);
    res.send(str);
    
  });

});

app.post('/queryReq', function(req , res) {

  //console.log("S: " + req.body.statement);
  connection.query(req.body.statement, function (err, rows, fields) {
   
    if (err) {
        console.log("ALERT: " + err);
    }else {
        
        res.send(rows);
    }
    

  });


});



