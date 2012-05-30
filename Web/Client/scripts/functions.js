      function getXDomainRequest() {
        var xdr = null;
        
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
          if (xdr.readyState == 4 && xdr.status == 200) {
            callbackok(xdr.responseText)
          } else {
            callbackerr(xdr);
          }
        };
        xdr.onerror = callbackfail;
        xdr.onprogress = callbackprogress;
        xdr.open("GET", addr);
        xdr.send();
      }

      function modLedState(no, etat){
        $("#status_led_"+no+" i").attr("class", "icon-refresh");
        var xdr = getXDomainRequest();
        xdr.onload = function(){
          if (xdr.responseText == "1"){
            $("#led_"+no)
              .html((etat == 0) ? "Off" : "On")
              .attr('onclick', (etat == 0) ? "modLedState(" + no + ", 1)" : "modLedState(" + no + ", 0)")
              .attr('class', (etat == 0) ? "btn btn-inverse" : "btn btn-success");

            $("#label_led_"+no).attr('onclick', (etat == 0) ? "modLedState(" + no + ", 1)" : "modLedState(" + no + ", 0)");
            
            $("#status_led_"+no+" i").attr("class", "icon-ok");
          } else {
            $("#status_led_"+no+" i").attr('class', 'icon-remove');
          }
        };
        xdr.open("GET", "http://"+ domain + ":" + port + "/l?n="+no+"&e="+etat+"&m="+mdp);
        xdr.send();
      }

      function modPcState(no){
        $("#status_pc_"+no+" i").attr("class", "icon-refresh");
        var xdr = getXDomainRequest();
        xdr.onload = function(){
          if (xdr.responseText == "1"){            
            $("#status_pc_"+no+" i").attr("class", "icon-ok");
          } else {
            $("#status_pc_"+no+" i").attr('class', 'icon-remove');
          }
        };
        xdr.open("GET", "http://"+ domain + ":" + port + "/o?n="+no+"&m="+mdp);
        xdr.send();
      }

      function modBarLedState(no, prevval, val){
        $("#bar_"+no+" i").attr('class', 'icon-refresh');
        var xdr = getXDomainRequest();
        xdr.onload = function(){
          if (xdr.responseText == "1"){
            $("#bar_"+no).children("button").attr("onclick", function(i){
              return (i > 0) ? "modBarLedState(" + no + ", " + val + ", " + (i-1) + ")" : false;
            });

            $($("#bar_"+no).children("button")[prevval+1]).attr("class", "btn");
            
            $($("#bar_"+no).children("button")[val+1])
              .attr("class", "btn btn-success active")
              .attr("onclick", "modBarLedState(" + no + ", " + val + ", " + val + ")");

            $("#bar_"+no+" i").attr('class', 'icon-ok');
          } else {
            $("#bar_"+no+" i").attr('class', 'icon-remove');
          }
        };
        xdr.open("GET", "http://"+ domain + ":" + port + "/b?n="+no+"&v="+val+"&m="+mdp);
        xdr.send();
      }

      function showLedState(json){
        for (i in json.l){
          if (json.l[i].e == "1") {
            $("#content_leds").html("<button class=\"btn disabled btn-primary label_led\" id=\"label_led_"+ json.l[i].n +"\" onclick=\"modLedState(" + json.l[i].n + ", 0)\"  >Led n° " + json.l[i].n + "</button>");
            $("#content_leds").append("<button class=\"btn btn-success\" id=\"led_"+ json.l[i].n +"\" onclick=\"modLedState(" + json.l[i].n + ", 0)\">On</button>");
          } else {
            $("#content_leds").html("<button class=\"btn disabled btn-primary label_led\" id=\"label_led_"+ json.l[i].n +"\" onclick=\"modLedState(" + json.l[i].n + ", 1)\"  >Led n° " + json.l[i].n + "</button>");
            $("#content_leds").append("<button class=\"btn btn-inverse\" id=\"led_"+ json.l[i].n +"\" onclick=\"modLedState(" + json.l[i].n + ", 1)\">Off</button>");
          }

          $("#content_leds").append("<button class=\"btn disabled\" id=\"status_led_"+ json.l[i].n +"\"><i class=\"icon-ok\"></i></button>");

          $("#content_leds").wrapInner("<div class=\"btn-group\"></div>");
          $("#content_leds").wrapInner("<div class=\"btn-toolbar\"></div>");
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
          if (i == 0){
            $("#content_bars").append("<button class=\"btn disabled btn-primary label_barled\" >Bar n° " + json.b[i].n + "</button>");
            
            for (j = 0 ; j <= json.b[i].m ; j++){
              if (j == json.b[i].v) {
                $("#content_bars").append("<button class=\"btn btn-success active\" onclick=\"modBarLedState(" + json.b[i].n + ", " + json.b[i].v + ", " + j + ")\">"+ j +"</button>");
              } else {
                $("#content_bars").append("<button class=\"btn\" onclick=\"modBarLedState(" + json.b[i].n + ", " + json.b[i].v + ", " + j + ")\">"+ j +"</button>");
              }
            }

            $("#content_bars").append("<a class=\"btn disabled\"><i class=\"icon-ok\"></i></a>");
            
            $("#content_bars")
              .wrapInner("<div id=\"bar_" + json.b[i].n + "\" class=\"btn-group\"></div>")
              .wrapInner("<div class=\"btn-toolbar\"></div>")
              .append("<div class=\"btn-toolbar\"><div id=\"bar_" + (json.b[i].n+1) + "\" class=\"btn-group\"></div></div>");
          } else {
            $("#bar_" +(json.b[i].n-1)).append("<button class=\"btn disabled btn-primary label_barled\" >Bar n° " + json.b[i].n + "</button>");
          
            for (j = 0 ; j <= json.b[i].m ; j++){
              if (j == json.b[i].v) {
                $("#bar_" +(json.b[i].n-1)).append("<button class=\"btn btn-success active\" onclick=\"modBarLedState(" + json.b[i].n + ", " + json.b[i].v + ", " + j + ")\">"+ j +"</button>");
              } else {
                $("#bar_" +(json.b[i].n-1)).append("<button class=\"btn\" onclick=\"modBarLedState(" + json.b[i].n + ", " + json.b[i].v + ", " + j + ")\">"+ j +"</button>");
              }
            }

            $("#bar_" +(json.b[i].n-1)).append("<a class=\"btn disabled\"><i class=\"icon-ok\"></i></a></div></div><div class=\"btn-toolbar\"><div id=\"bar_" + (json.b[i].n+1) + "\" class=\"btn-group\"></div></div>");
          
          }
        }
      }

      function main(){
        if(!(loaded))
          $("#loading").show();

        $("eegg").html('');
        $("#loading_bar").css("width", "5%");
        $("#status").attr("class", "icon-refresh");

        GetData("http://"+ domain + ":" + port + "/h?m=" + mdp, 
          function(html){
            $("#loading_bar").css("width", "100%");
            
            var json = JSON.parse(html);

            showBarLedState(json);
            showLedState(json);
            showPcState(json)

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
          },
          function(){
            retardeur = setTimeout(main, 10000);

            $("#notifications").prepend("<div class=\"alert alert-error\"><button class=\"close\" data-dismiss=\"alert\">×</button>Aucune réponse reçue, l'arduino est probablement eteint ...</div>")
          },
          function(i){
            if(i.totalSize < 0) 
              $("#loading_bar").css("width", (((i.progress/i.totalSize) * 95) + 5) + "%");
          }
        );
      }