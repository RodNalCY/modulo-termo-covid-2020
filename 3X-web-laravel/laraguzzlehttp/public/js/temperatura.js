var global_value_temperatura = 0;
var global_value_mensaje ="";
var global_value_status ="";
var global_value_request_verify ;
////////////////////////////////TABLE-CHARTS////////////////////////////////////////////
 google.charts.load('current', {'packages':['table']});
 google.charts.setOnLoadCallback(drawTable);

 function drawTable() {
   var data = new google.visualization.DataTable();
   data.addColumn('string', 'Estado');
   data.addColumn('number', 'Messaje');
   data.addColumn('boolean', 'API Request');
   data.addRows([
     [global_value_status,  {v: 10000, f: global_value_mensaje}, global_value_request_verify]
   ]);

   var table = new google.visualization.Table(document.getElementById('table_div'));

   table.draw(data, {width: '100%', height: '100%'});
 }
////////////////////////////////CARGA-CHARTS////////////////////////////////////////////
 google.charts.load('current', {'packages':['gauge']});
 google.charts.setOnLoadCallback(drawChart);

      function drawChart() {

        var data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['T. °C', global_value_temperatura]
        ]);

        var options = {
          greenFrom: 36, greenTo: 37.8,
          yellowFrom:37.8, yellowTo: 39,
          redFrom: 39, redTo: 40,
          minorTicks: 5,
          max:40,
          min: 35
        };

        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));

        chart.draw(data, options);
}
////////////////////////////////CARGA-DATA////////////////////////////////////////////
function cargarDatos(){

    $.get("apitemperatura", function (datos, state) {
        var content = JSON.parse(datos);
        console.log(content);
        if(content["message"]){
           global_value_status         = content["status"];
           global_value_mensaje        = content["message"];
           global_value_request_verify = true;
           global_value_temperatura = 0;
           drawTable();
           drawChart();
        }else{
           global_value_status         = content["status"];
           global_value_mensaje        = "Medición Correcta";
           global_value_request_verify = true;
           global_value_temperatura = parseFloat(content["temperatura"]);
           drawChart();
           drawTable();
        }


    });

}

