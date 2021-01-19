$(document).ready(function() {
 
    $('#helpTitle').on('shown.bs.collapse', function() {
       
       bothTables();

    }).on('show.bs.collapse', function() {
    });


    $.ajax({
        type: 'get',            
        dataType: 'text',       
        url: '/getFileList',   
        success: function (data) {
           
            makeList(data);
            makeFileLog(data);

        },
        fail: function(error) {
            
            console.log(error);
        }
    });


    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#subform').submit(function(e){
        e.preventDefault();

        var fName = document.getElementById("myFile").value;

        if(fName == "") {

            alert("Please choose a file.");

        }else {

            var data = new FormData(jQuery('#subform')[0]);

            $.ajax({

                type: 'post',
                url: '/upload',
                data: data,
                contentType: false,
                processData: false,
                success: function (data) {
                
                },
                fail: function(error) {
                
                    console.log(error);
                }

            });

            update();
        }
    });

    $('#createFileForm').submit(function(e){
        e.preventDefault();

        var fName = document.forms["createFileForm"]["fileName"].value;
        var source = document.forms["createFileForm"]["source"].value;
        var sName = document.forms["createFileForm"]["subName"].value;
        var encoding = document.forms["createFileForm"]["encoding"].value;
        var vers = document.forms["createFileForm"]["gedVers"].value;

        if(fName == "" || source == "" || sName == "" || encoding == "" || vers == "") {

            alert("Please Fill All Required Fields.");

        }else {

            var data = new FormData(jQuery('#createFileForm')[0]);

            $.ajax({

                type: 'post',
                url: '/createFile',
                data: data,
                contentType: false,
                processData: false,
                success: function (data) {
               
                },
                fail: function(error) {
                
                    console.log(error);
                }
            });

            update();

            var filename = document.getElementById("fileID").value;
            document.getElementById("statusPanel").innerHTML += "Saved a new file: " + filename + "<br>";
        }
    });

    $('#addIndiForm').submit(function(e){
        e.preventDefault();

        var first = document.forms["addIndiForm"]["givenName"].value;
        var second = document.forms["addIndiForm"]["surname"].value;

        var file = document.getElementById("filePick").innerHTML;

        if(first == null || first == "" || second == null || second == "" || file == null || file == "") {

            alert("Please Fill All Fields.");

        }else {

            var data = new FormData(jQuery('#addIndiForm')[0]);
            data.append('filename', file);

            $.ajax({

                type: 'post',
                url: '/addIndi',
                data: data,
                contentType: false,
                processData: false,
                success: function (datas) {
               
                    if(datas != "FAIL") {

                        document.getElementById("statusPanel").innerHTML += " " + datas + file + "<br>";

                    }else if(datas == "FAIL") {

                        document.getElementById("statusPanel").innerHTML += "Error: " + file + "<br>";
                    }
                },
                fail: function(error) {
                    
                    console.log(error);
                }
            });

            update();
        }
    });

    $('#getDescForm').submit(function(e){
        e.preventDefault();

        var first = document.forms["getDescForm"]["givenName"].value;
        var second = document.forms["getDescForm"]["surname"].value;

        var file = document.getElementById("filePick2").innerHTML;

        if(first == null || first == "" || second == null || second == "" || file == null || file == "") {

            alert("Please Fill All Fields.");

        }else {

            var data = new FormData(jQuery('#getDescForm')[0]);
            data.append('filename', file);

            $.ajax({

                type: 'post',
                url: '/getDesc',
                data: data,
                contentType: false,
                processData: false,
                success: function (datas) {
               
                    descendantsTable(datas);

                },
                fail: function(error) {
                    
                    console.log(error);
                }
            });

            update();
        }
    });

    $('#getAncsForm').submit(function(e){
        e.preventDefault();

        var first = document.forms["getAncsForm"]["givenName"].value;
        var second = document.forms["getAncsForm"]["surname"].value;

        var file = document.getElementById("filePick3").innerHTML;

        if(first == null || first == "" || second == null || second == "" || file == null || file == "") {

            alert("Please Fill All Fields.");

        }else {

            var data = new FormData(jQuery('#getAncsForm')[0]);
            data.append('filename', file);

            $.ajax({

                type: 'post',
                url: '/getAncs',
                data: data,
                contentType: false,
                processData: false,
                success: function (datas) {
               
                    ancestorsTable(datas);

                },
                fail: function(error) {
                    
                    console.log(error);
                }
            });

            update();
        }
    });

    document.getElementById("btnStore").disabled = true;
    document.getElementById("btnDisplay").disabled = true;
    document.getElementById("btnClear").disabled = true;
    document.getElementById("btnQueryDrop").disabled = true;
    document.getElementById("btnHelp").disabled = true;
    document.getElementById("btnSubmit").disabled = true;

    $('#connectDBForm').submit(function(e){
        e.preventDefault();

        var user = document.forms["connectDBForm"]["username"].value;
        var pass = document.forms["connectDBForm"]["password"].value;
        var database = document.forms["connectDBForm"]["database"].value;

        if(user == "" || pass == "" || database == "") {

            alert("Please Fill All Fields.");

        }else {

            var check = document.getElementById("failText");

            if(check != null) {

                check.remove();
            }

            var load = document.createElement("DIV");
            load.setAttribute("class", "loader");
            document.getElementById("footer").prepend(load);

            var data = new FormData(jQuery('#connectDBForm')[0]);

            $.ajax({

                type: 'post',
                url: '/conDB',
                data: data,
                contentType: false,
                processData: false,
                success: function (datas) {
               
                    if(datas == "OK") {

                        var exit = document.getElementById("closePop");
                        exit.click();
                        load.remove(load);
                        document.getElementById("btnDB").disabled = true;
                        document.getElementById("btnStore").disabled = false;
                        document.getElementById("btnDisplay").disabled = false;
                        document.getElementById("btnClear").disabled = false;
                        document.getElementById("btnQueryDrop").disabled = false;
                        document.getElementById("btnHelp").disabled = false;
                        document.getElementById("btnSubmit").disabled = false;

                    }else if(datas == "FAIL") {

                        load.remove(load);
                        var text = document.createElement("p");
                        text.id = "failText";
                        var node = document.createTextNode("Login failed, please try again.");
                        text.appendChild(node);
                        document.getElementById("footer").prepend(text);
                    }
                },
                fail: function(error) {
                
                    console.log(error);
                }
            });
        }
    });

    $('#storeFilesForm').submit(function(e){
        e.preventDefault();

        $.ajax({
            type: 'get',           
            dataType: 'text',      
            url: '/storeFiles',  
            success: function (data) {
               
               DBstatus();
            },
            fail: function(error) {
                
                console.log(error); 
            }
        });
    });

    $('#clearDataForm').submit(function(e){
        e.preventDefault();

        $.ajax({
            type: 'get',           
            dataType: 'text',      
            url: '/clearData',  
            success: function (data) {
               
               DBstatus();
            },
            fail: function(error) {
                
               console.log(error);
            }
        });
    });

    $('#displayDataForm').submit(function(e){
        e.preventDefault();

        DBstatus();

    });
});

function update() {

    $(document).ready(function() {
    
        $.ajax({
            type: 'get',            
            dataType: 'text',       
            url: '/getFileList',   
            success: function (data) {
               
                makeList(data);
                makeFileLog(data);
            
            },
            fail: function(error) {
                
                console.log(error);
            }
        });
    });

    $('#fileLogTable').empty();
    $('#GEDviewDrop').empty();
    $('#addIndiDrop').empty();
    $('#decsDrop').empty();
    $('#ancsDrop').empty();

}

function clearPanel() { 

    document.getElementById("statusPanel").innerHTML = "";
}

document.getElementById("upBtn").onclick = function() {

    var filename = document.getElementById("myFile").value;
    filename = filename.replace(/.*[\/\\]/, '');

    if(filename != "") {
        document.getElementById("statusPanel").innerHTML += "Uploaded File: " + filename + "<br>";
    }
}

function descendantsTable(data) {

    $('#descTable').empty();

    var array = JSON.parse(data);

    if(array.length > 0) {

        var element = document.getElementById("cardBody3");

        if(element != null) {
            element.remove();
        }

        var table = document.getElementById("descTable");

        var head = table.createTHead();
        var hRow = head.insertRow(0);
        var hCell = hRow.insertCell(0);
        hCell.innerHTML = "<b>Descendants</b>";

        for(i = 0; i < array.length; i++) {

            var row = table.insertRow(i+1);
            var cell = row.insertCell(0);

            for(j = 0; j < array[i].length; j++) {
        
                cell.innerHTML += array[i][j].givenName + " " + array[i][j].surname + " | ";
            }
        }

    }else {

        var cardBody = document.createElement("DIV");
        cardBody.setAttribute("class", "card-body");
        cardBody.innerHTML = "No Descendants.";
        cardBody.id = "cardBody3"

        document.getElementById("card3").appendChild(cardBody);        
    }
}

function ancestorsTable(data) {

    $('#ancsTable').empty();

    var array = JSON.parse(data);

    if(array.length > 0) {

        var element = document.getElementById("cardBody4");

        if(element != null) {
            element.remove();
        }

        var table = document.getElementById("ancsTable");

        var head = table.createTHead();
        var hRow = head.insertRow(0);
        var hCell = hRow.insertCell(0);
        hCell.innerHTML = "<b>Ancestors</b>";

        for(i = 0; i < array.length; i++) {

            var row = table.insertRow(i+1);
            var cell = row.insertCell(0);

            for(j = 0; j < array[i].length; j++) {
        
                cell.innerHTML += array[i][j].givenName + " " + array[i][j].surname + " | ";
            }
        }

    }else {

        var cardBody = document.createElement("DIV");
        cardBody.setAttribute("class", "card-body");
        cardBody.innerHTML = "No Ancestors.";
        cardBody.id = "cardBody4"

        document.getElementById("card4").appendChild(cardBody);
    }
}

function makeList(file) {

    var array = JSON.parse(file);

    for(i = 0; i < array.length; i++) {

        var link = document.createElement("a");
        link.setAttribute("class", "dropdown-item");
        link.href = "#";
        link.innerHTML = array[i];
        link.setAttribute("onclick", "makeGEDview(\"" + array[i] + "\")");

        document.getElementById("GEDviewDrop").appendChild(link);
    }

    for(i = 0; i < array.length; i++) {

        var link = document.createElement("a");
        link.setAttribute("class", "dropdown-item");
        link.href = "#";
        link.innerHTML = array[i];
        link.setAttribute("onclick", "filePick(\"" + array[i] + "\")");

        document.getElementById("addIndiDrop").appendChild(link);
    }

    for(i = 0; i < array.length; i++) {

        var link = document.createElement("a");
        link.setAttribute("class", "dropdown-item");
        link.href = "#";
        link.innerHTML = array[i];
        link.setAttribute("onclick", "filePick2(\"" + array[i] + "\")");

        document.getElementById("decsDrop").appendChild(link);
    }

    for(i = 0; i < array.length; i++) {

        var link = document.createElement("a");
        link.setAttribute("class", "dropdown-item");
        link.href = "#";
        link.innerHTML = array[i];
        link.setAttribute("onclick", "filePick3(\"" + array[i] + "\")");

        document.getElementById("ancsDrop").appendChild(link);
    }

}

function makeFileLog(file) {

    var array = JSON.parse(file);

    if(array.length > 0) {

        var element = document.getElementById("cardBody1");

        if(element != null) {
            element.remove();
        }

        var table = document.getElementById("fileLogTable");

        var head = table.createTHead();
        var hRow = head.insertRow(0);

        for(j = 0; j < 8; j++) {

            var hCell = hRow.insertCell(-1);
        }

        table.rows[0].cells[0].innerHTML = "<b>File Name <br>(Click to Download)</b>";
        table.rows[0].cells[1].innerHTML = "<b>Source</b>";
        table.rows[0].cells[2].innerHTML = "<b>GEDC Version</b>";
        table.rows[0].cells[3].innerHTML = "<b>Encoding</b>";
        table.rows[0].cells[4].innerHTML = "<b>Submitter Name</b>";
        table.rows[0].cells[5].innerHTML = "<b>Submitter Address</b>";
        table.rows[0].cells[6].innerHTML = "<b>Number of Individuals<b>";
        table.rows[0].cells[7].innerHTML = "<b>Number of Families</b>";


        for(i = 0; i < array.length; i++) {

            var link = document.createElement("a");
            link.href = "/uploads/" + array[i];
            link.innerHTML = array[i];

            var row = table.insertRow(i+1);
            var cell;

            for(j = 0; j < 8; j++) {

                cell = row.insertCell(-1);
            }

            table.rows[i+1].cells[0].appendChild(link);

            var count = 1;

            var data1 = new FormData();
            data1.append('filename', array[i]);

            $.ajax({
                type: 'post',
                url: '/makeSummary',  
                data: data1,
                contentType: false,
                processData: false,
                success: function (data) {
               
                    //console.log(data); 
                    var fileSum = JSON.parse(data);
                    
                    
                    table.rows[count].cells[1].innerHTML = fileSum.source;
                    table.rows[count].cells[2].innerHTML = fileSum.GEDCvers;
                    table.rows[count].cells[3].innerHTML = fileSum.encoding;
                    table.rows[count].cells[4].innerHTML = fileSum.subName;
                    table.rows[count].cells[5].innerHTML = fileSum.subAddr;
                    table.rows[count].cells[6].innerHTML = fileSum.numInd;
                    table.rows[count].cells[7].innerHTML = fileSum.numFam;
                    
                    ++count;

                },
                fail: function(error) {
                
                    console.log(error);
                }
            });

            
        }

    }else {

        var cardBody = document.createElement("DIV");
        cardBody.setAttribute("class", "card-body");
        cardBody.innerHTML = "No Files Available.";
        cardBody.id = "cardBody1";

        document.getElementById("card1").appendChild(cardBody);
    }


}

function makeGEDview(filename) {
  
    var data1 = new FormData();
    data1.append('filename', filename);

    $.ajax({
        type: 'post',
        url: '/makeView',  
        data: data1,
        contentType: false,
        processData: false,
        success: function (data) {
       
            if(data != "") {

                var element = document.getElementById("cardBody2");

                if(element != null) {
                    element.remove();
                }

                var viewArray = JSON.parse(data);

                var table = document.getElementById("GEDviewTable");

                var head = table.createTHead();
                var hRow = head.insertRow(0);

                for(j = 0; j < 4; j++) {

                    var hCell = hRow.insertCell(-1);
                }

                table.rows[0].cells[0].innerHTML = "<b>Given Name</b>";
                table.rows[0].cells[1].innerHTML = "<b>Surname</b>";
                table.rows[0].cells[2].innerHTML = "<b>Sex</b>";
                table.rows[0].cells[3].innerHTML = "<b>Family Size</b>";

                for(i = 0; i < viewArray.length; i++) {

                    var row = table.insertRow(-1);
                    var cell;

                    for(j = 0; j < 4; j++) {

                       cell = row.insertCell(-1);
                    }


                    table.rows[i+1].cells[0].innerHTML = viewArray[i].givenName;
                    table.rows[i+1].cells[1].innerHTML = viewArray[i].surname;
                    table.rows[i+1].cells[2].innerHTML = viewArray[i].gender;
                    table.rows[i+1].cells[3].innerHTML = viewArray[i].famSize;

                }

            }else {

                var cardBody = document.createElement("DIV");
                cardBody.setAttribute("class", "card-body");
                cardBody.innerHTML = "No Individuals.";
                cardBody.id = "cardBody2";

                document.getElementById("card2").appendChild(cardBody);
            }
            
            

        },
        fail: function(error) {
        
            console.log(error);
        }
    });

    $('#GEDviewTable').empty();

}

function filePick(filename) {

    document.getElementById("filePick").innerHTML = filename;
}

function filePick2(filename) {

    document.getElementById("filePick2").innerHTML = filename;
}

function filePick3(filename) {

    document.getElementById("filePick3").innerHTML = filename;
}

function DBstatus() {

    var numFiles;
    var numInds

    $.ajax({
        type: 'get',           
        dataType: 'text',      
        url: '/fileCount',  
        success: function (data) {

           document.getElementById("statusPanel").innerHTML += "Database has " + data + " files ";
           numFiles = data;

           $.ajax({
                type: 'get',           
                dataType: 'text',      
                url: '/indCount',  
                success: function (data) {

                    document.getElementById("statusPanel").innerHTML += "and " + data + " individuals. <br>";
                    numInds = data;

                    if(numFiles == 0 && numInds == 0) {

                        document.getElementById("btnClear").disabled = true;

                    }else {

                        document.getElementById("btnClear").disabled = false;
                    }
                },
                fail: function(error) {
                    
                   console.log(error);
                }

            });

        },
        fail: function(error) {
            
           console.log(error);
        }

    });
}


function queryOne() {

    var data1 = new FormData();
    data1.append('statement', "SELECT * FROM INDIVIDUAL ORDER BY surname");

    queryIndTable(data1);

}

function queryTwo() {

    document.getElementById("queryTA").value = "SELECT * FROM INDIVIDUAL WHERE INDIVIDUAL.source_file = (SELECT file_id FROM FILE WHERE FILE.file_Name = \'*FILE NAME GOES HERE*\')";
}

function queryThree() {

    document.getElementById("queryTA").value = "SELECT * FROM INDIVIDUAL WHERE INDIVIDUAL.surname = \'*SURNAME GOES HERE*\'";
}

function queryFour() {

    document.getElementById("queryTA").innerHTML = "SELECT * FROM INDIVIDUAL WHERE INDIVIDUAL.sex = \'*SEX GOES HERE (M/F)*\'";
}

function queryFive() {

    document.getElementById("queryTA").value = "SELECT * FROM FILE WHERE FILE.num_families > *NUMBER GOES HERE*";
}

function submitQuery() {

    var theQuery = document.getElementById("queryTA").value;

    var data = new FormData();
    data.append('statement', theQuery);

    var pos = -1;
    pos = theQuery.search("INDIVIDUAL");

    if(pos != -1) {

        queryIndTable(data);

    }else {

        queryFileTable(data);
    }

}

function queryIndTable(query) {

    $('#resultTable').empty();

    $.ajax({

        type: 'post',
        url: '/queryReq',  
        data: query,
        contentType: false,
        processData: false,
        success: function (data) {

            if(data != "") {

                var table = document.getElementById("resultTable");

                var head = table.createTHead();
                var hRow = head.insertRow(0);

                for(j = 0; j < 4; j++) {

                    var hCell = hRow.insertCell(-1);
                }

                table.rows[0].cells[0].innerHTML = "<b>Given Name</b>";
                table.rows[0].cells[1].innerHTML = "<b>Surname</b>";
                table.rows[0].cells[2].innerHTML = "<b>Sex</b>";
                table.rows[0].cells[3].innerHTML = "<b>Family Size</b>";

                for(i = 0; i < data.length; i++) {

                    var row = table.insertRow(-1);
                    var cell;

                    for(j = 0; j < 4; j++) {

                       cell = row.insertCell(-1);
                    }


                    table.rows[i+1].cells[0].innerHTML = data[i].given_name;
                    table.rows[i+1].cells[1].innerHTML = data[i].surname;
                    table.rows[i+1].cells[2].innerHTML = data[i].sex;
                    table.rows[i+1].cells[3].innerHTML = data[i].fam_size;

                }

            } 

        },
        fail: function(error) {
            
            console.log(error);
        }
    });
}

function queryFileTable(query) {

    $('#resultTable').empty();

    $.ajax({
        type: 'post',
        url: '/queryReq',  
        data: query,
        contentType: false,
        processData: false,
        success: function (data) {
        
            if(data != "") {

                var table = document.getElementById("resultTable");

                var head = table.createTHead();
                var hRow = head.insertRow(0);

                for(j = 0; j < 8; j++) {

                    var hCell = hRow.insertCell(-1);
                }

                table.rows[0].cells[0].innerHTML = "<b>File Name</b>";
                table.rows[0].cells[1].innerHTML = "<b>Source</b>";
                table.rows[0].cells[2].innerHTML = "<b>GEDC Version</b>";
                table.rows[0].cells[3].innerHTML = "<b>Encoding</b>";
                table.rows[0].cells[4].innerHTML = "<b>Submitter Name</b>";
                table.rows[0].cells[5].innerHTML = "<b>Submitter Address</b>";
                table.rows[0].cells[6].innerHTML = "<b>Number of Individuals<b>";
                table.rows[0].cells[7].innerHTML = "<b>Number of Families</b>";

                for(i = 0; i < data.length; i++) {

                    var row = table.insertRow(-1);
                    var cell;

                    for(j = 0; j < 8; j++) {

                       cell = row.insertCell(-1);
                    }

                    table.rows[i+1].cells[0].innerHTML = data[i].file_Name;
                    table.rows[i+1].cells[1].innerHTML = data[i].source
                    table.rows[i+1].cells[2].innerHTML = data[i].version
                    table.rows[i+1].cells[3].innerHTML = data[i].encoding;
                    table.rows[i+1].cells[4].innerHTML = data[i].sub_name;
                    table.rows[i+1].cells[5].innerHTML = data[i].sub_addr;
                    table.rows[i+1].cells[6].innerHTML = data[i].num_individuals;
                    table.rows[i+1].cells[7].innerHTML = data[i].num_families;

                }
            }         
        },
        fail: function(error) {
        
            console.log(error);
        }
    });

}

function bothTables() {

    describeFileTable();
    describeIndTable();
}

function describeFileTable() {

    $('#helpFileTable').empty();

    var data = new FormData();
    data.append('statement', "DESCRIBE FILE");

    $.ajax({

        type: 'post',
        url: '/queryReq',  
        data: data,
        contentType: false,
        processData: false,
        success: function (data) {

            if(data != "") {

                var table = document.getElementById("helpFileTable");

                var head = table.createTHead();
                var hRow = head.insertRow(0);

                for(j = 0; j < 2; j++) {

                    var hCell = hRow.insertCell(-1);
                }

                table.rows[0].cells[0].innerHTML = "<b>FILE: Column Name</b>";
                table.rows[0].cells[1].innerHTML = "<b>Type</b>";

                for(i = 0; i < data.length; i++) {

                    var row = table.insertRow(-1);
                    var cell;

                    for(j = 0; j < 2; j++) {

                       cell = row.insertCell(-1);
                    }

                    table.rows[i+1].cells[0].innerHTML = data[i].Field;
                    table.rows[i+1].cells[1].innerHTML = data[i].Type;

                }
            } 
        },
        fail: function(error) {
        
            console.log(error);
        }
    });
}

function describeIndTable() {

    $('#helpIndTable').empty();

    var data = new FormData();
    data.append('statement', "DESCRIBE INDIVIDUAL");

    $.ajax({

        type: 'post',
        url: '/queryReq',  
        data: data,
        contentType: false,
        processData: false,
        success: function (data) {

            if(data != "") {

                var table = document.getElementById("helpIndTable");

                var head = table.createTHead();
                var hRow = head.insertRow(0);

                for(j = 0; j < 2; j++) {

                    var hCell = hRow.insertCell(-1);
                }

                table.rows[0].cells[0].innerHTML = "<b>IND: Column Name</b>";
                table.rows[0].cells[1].innerHTML = "<b>Type</b>";

                for(i = 0; i < data.length; i++) {

                    var row = table.insertRow(-1);
                    var cell;

                    for(j = 0; j < 2; j++) {

                       cell = row.insertCell(-1);
                    }


                    table.rows[i+1].cells[0].innerHTML = data[i].Field;
                    table.rows[i+1].cells[1].innerHTML = data[i].Type;

                }

            } 

        },
        fail: function(error) {
            
            console.log(error);
        }
    });
}






