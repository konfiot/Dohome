var sliding = false, rip = false;

function getXDomainRequest() {
  var xdr = null;
  
  rip = true;

  if (window.XDomainRequest) {
    xdr = new XDomainRequest(); 
  } else if (window.XMLHttpRequest) {
    xdr = new XMLHttpRequest(); 
  } else {
    alert("Votre navigateur ne gère pas l'AJAX cross-domain !");
  }
  
  return xdr;        
}


function GetData(addr, callbackok, callbackerr, callbackfail, callbackprogress) {
  var xdr = getXDomainRequest();
  xdr.timeout = 5000;
  xdr.onload = function(){
    rip = false;
    if (xdr.readyState == 4 && xdr.status == 200) {
      callbackok(xdr.responseText)
    } else {
      callbackerr(xdr);
    }
  };
  xdr.onerror = function(){
    rip = false;
    callbackfail();
  }
  xdr.onprogress = callbackprogress;
  xdr.open("GET", addr);
  xdr.send();
}

function modLedState(no, etat){
  var xdr = getXDomainRequest();
  xdr.open("GET", "http://"+ domain + ":" + port + "/l?n="+no+"&e="+((etat) ? 1 : 0)+"&m="+mdp);
  xdr.send();
  xdr.onload = function(){
    rip = false;
  };
}

function modPcState(no){
  $("#status_pc_"+no+" i").attr("class", "icon-refresh");
  var xdr = getXDomainRequest();
  xdr.onload = function(){
    rip = false;
    if (xdr.responseText == "1"){            
      $("#status_pc_"+no+" i").attr("class", "icon-ok");
    } else {
      $("#status_pc_"+no+" i").attr('class', 'icon-remove');
    }
  };
  xdr.open("GET", "http://"+ domain + ":" + port + "/o?n="+no+"&m="+mdp);
  xdr.send();
}

function modBarLedState(no, val){
  var xdr = getXDomainRequest();
  xdr.onload = function(){
    rip = false;
  };
  xdr.open("GET", "http://"+ domain + ":" + port + "/b?n="+no+"&v="+val+"&m="+mdp);
  xdr.send();
}

function showLedState(json){
  $("#content_leds").html('');
  
  for (i in json.l){
    $("#content_leds").append(json.l[i].n + '<div id="button_' + json.l[i].n + '"></div>');
    $('#button_' + json.l[i].n).jqxSwitchButton({ height: 27, width: 81, theme: 'classic', checked: (json.l[i].e == "1") ? true : false});
    $('#button_' + json.l[i].n).bind('change', function(event){
      modLedState(event.target.id.split('_')[1], event.args.check);
    });
  }
}

function showPcState(json){
  $("#content_wol").html('');
  for (i in json.p){
    $("#content_wol").append("<div class=\"btn-toolbar\"></div><div class=\"btn-group\"><button class=\"btn disabled btn-primary label_pc\" id=\"label_pc_"+ json.p[i].n +"\" onclick=\"modPcState(" + json.p[i].n + ", 0)\">" + json.p[i].l + "</button><button class=\"btn btn-success\" id=\"pc_"+ json.p[i].n +"\" onclick=\"modPcState(" + json.p[i].n + ")\">Wake up !</button><button class=\"btn disabled\" id=\"status_pc_"+ json.p[i].n +"\"><i class=\"icon-ok\"></i></button></div></div>");
  }
}

function showBarLedState(json){
  $("#content_bars").html('');


  for (i in json.b){
    $("#content_bars").append(json.b[i].n + '<div id="bar_' + json.b[i].n + '"></div>');
    $("#bar_" + json.b[i].n).jqxSlider({ min: 0, max: json.b[i].m, ticksFrequency: 1, step: 1, theme: 'classic', mode: "fixed" });
    $("#bar_" + json.b[i].n).jqxSlider('setValue', json.b[i].v * 1);

    $("#bar_" + json.b[i].n).bind('change', function(event){
      if (!(sliding)){
        modBarLedState(event.target.id.split('_')[1], event.args.value);
      }
    });

    $("#bar_" + json.b[i].n).bind('slideStart', function(event){
      sliding = true;
    });

    $("#bar_" + json.b[i].n).bind('slideEnd', function(event){
      sliding = false;
      modBarLedState(event.target.id.split('_')[1], event.args.value);
    });
  }
}

function refreshBarLedState(json){
  if((!(sliding)) && (!(rip))){
    for (i in json.b){
      if(($("#bar_" + json.b[i].n).jqxSlider('getValue')) != (json.b[i].v * 1)){
        $("#bar_" + json.b[i].n).jqxSlider('setValue', json.b[i].v * 1);
      }
    }
  }
}

function main(){
  $('#docking').jqxDocking({ theme: 'classic', orientation: 'horisontal', width: $('.span12').css('width'), mode: 'docked' });
  $('#docking').jqxDocking('hideAllCloseButtons');

  if(!(loaded))
    $("#loading").show();

  $("eegg").html('');
  $("#loading_bar").css("width", "5%");
  $("#status").attr("class", "icon-refresh");

  GetData("http://"+ domain + ":" + port + "/h?m=" + mdp, 
    function(html){
      $("#loading_bar").css("width", "100%");
      
      var json = JSON.parse(html);
      if(!(loaded)){
        showBarLedState(json);
        showLedState(json);
        showPcState(json)
      } else {
        refreshBarLedState(json);
        /*refreshLedState(json);
        refreshPcState(json)*/
      }

      if(!(loaded)){
        loaded = true;
        $("#loading").fadeOut();
        $("#controle").slideDown();
      }

      $("#uptime").html("Allumé depuis : " + Math.floor(json.up / 86400, 1) + "J " + (Math.floor(json.up / 3600, 1) % 24) + "H " + (Math.floor(json.up / 60, 1) % 60) + "m " + (json.up % 60) + "s");
      $("#status").attr("class", "icon-ok");

      retardeur = setTimeout(main, 1000);
    }, 
    function(xdr){
      retardeur = setTimeout(main, 5000);

      $("#notifications").prepend("<div class=\"alert alert-error\"><button class=\"close\" data-dismiss=\"alert\">×</button>Erreur " + xdr.status + " : " + xdr.statusText + "</div>");
      $("#status").attr("class", "icon-remove");
    },
    function(){
      retardeur = setTimeout(main, 10000);

      $("#notifications").prepend("<div class=\"alert alert-error\"><button class=\"close\" data-dismiss=\"alert\">×</button>Aucune réponse reçue, l'arduino est probablement eteint ...</div>")
      $("#status").attr("class", "icon-remove");
    },
    function(i){
      if(i.totalSize > 0) 
        $("#loading_bar").css("width", (((i.progress/i.totalSize) * 95) + 5) + "%");
    });
}