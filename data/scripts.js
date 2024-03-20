$(function () {
    // Side Bar Toggle
    $('.hide-sidebar').click(function () {
        $('#sidebar').hide('fast', function () {
            $('#content').removeClass('span9');
            $('#content').addClass('span12');
            $('.hide-sidebar').hide();
            $('.show-sidebar').show();
        });
    });

    $('.show-sidebar').click(function () {
        $('#content').removeClass('span12');
        $('#content').addClass('span9');
        $('.show-sidebar').hide();
        $('.hide-sidebar').show();
        $('#sidebar').show('fast');
    });
});

//WebSocket Conexión............ le agregue el puerto 84

var connection = new WebSocket('ws://' + location.hostname + ':84' + '/ws', ['arduino']);

//Abrir el WebSocket
connection.onopen = function () {
    console.log('WS/ Connectado');
};

//Error
connection.onerror = function (error) {
    console.log('WS/ Error ', error);
};

//Cuando llega Mensaje llamamos la Función ProcessData
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
    processData(e.data);
};

//Cerrar el Websocket
connection.onclose = function () {
    console.log('WS/ Cerrado');
};
//Camturar el Pathname del URL
let pathname = window.location.pathname;

function processData(data) {

    //Convertir la Dara en Json
    let json = JSON.parse(data);

    //Si estamos en el dashboard actualizamos los valores
    if (pathname == "/") {

        if (json.WFEstatus != null) {

            let WFEstatus = document.getElementById('WFEstatus');
            let MQTTStatus = document.getElementById('MQTTStatus');
            let MQTTBroker = document.getElementById('MQTTBroker');
            let WFDBM = document.getElementById('WFDBM');
            let WFPRC = document.getElementById('WFPRC');
            let WiFiChart = document.getElementById('WiFiChart');
            let PMEMChart = document.getElementById('PMEMChart');
            let ramChart = document.getElementById('ramChart');
            let cpuChart = document.getElementById('cpuChart');
            let TemperaturaH1 = document.getElementById('TemperaturaH1');
            let TemperaturaH2 = document.getElementById('TemperaturaH2');
            let TemperaturaAmbiente = document.getElementById('TemperaturaAmbiente');
            //let TensionGeneral = document.getElementById('TensionGeneral');
            //let TensionH1 = document.getElementById('TensionH1');
            //let TensionH2 = document.getElementById('TensionH2');
            //let magnetismoInterno = document.getElementById('magnetismoInterno');
            let EstadoDeSistema = document.getElementById('EstadoDeSistema');
            let AlarmaIntrusion = document.getElementById('AlarmaIntrusion');
            let AlarmaH1 = document.getElementById('AlarmaH1');
            let AlarmaH2 = document.getElementById('AlarmaH2');
            let EstadoPuerto0 = document.getElementById('EstadoPuerto0');
            let EstadoPuerto1 = document.getElementById('EstadoPuerto1');
            let EstadoPuerto2 = document.getElementById('EstadoPuerto2');
            let Corriente1 = document.getElementById('Corriente1');
            let Corriente2 = document.getElementById('Corriente2');
            let Gas = document.getElementById('Gas');

            let dia = document.getElementById('dia');
            let mes = document.getElementById('mes');
            let ano = document.getElementById('ano');
            let horas = document.getElementById('horas');
            let minutos = document.getElementById('minutos');
            let segundos = document.getElementById('segundos');








            WFEstatus.innerHTML = json.WFEstatus;
            MQTTStatus.innerHTML = json.MQTTStatus;
            MQTTBroker.innerHTML = json.MQTTBroker;
            WFDBM.innerHTML = json.WFDBM + " dBm";
            WFPRC.innerHTML = json.WFPRC + " %";
            WiFiChart.innerHTML = json.WFPRC + " %";
            PMEMChart.innerHTML = json.PMEM + " %";
            ramChart.innerHTML = json.ram + " %";
            cpuChart.innerHTML = json.cpu + " °C";
            TemperaturaH1.innerHTML = json.TemperaturaH1;
            TemperaturaH2.innerHTML = json.TemperaturaH2;
            TemperaturaAmbiente.innerHTML = json.TemperaturaAmbiente;
           // TensionGeneral.innerHTML = json.TensionGeneral;
            //TensionH1.innerHTML = json.TensionH1;
            //TensionH2.innerHTML = json.TensionH2;
            //magnetismoInterno.innerHTML = json.magnetismoInterno;
            EstadoDeSistema.innerHTML = json.EstadoDeSistema;
            AlarmaIntrusion.innerHTML = json.AlarmaIntrusion;
            AlarmaH1.innerHTML = json.AlarmaH1;
            AlarmaH2.innerHTML = json.AlarmaH2;
            EstadoPuerto0.innerHTML = json.EstadoPuerto0;
            EstadoPuerto1.innerHTML = json.EstadoPuerto1;
            EstadoPuerto2.innerHTML = json.EstadoPuerto2;
            //Corriente1.innerHTML = json.Corriente1;
            //Corriente2.innerHTML = json.Corriente2;
            //Gas.innerHTML = json.Gas;

            horas.innerHTML = json.horas;
            minutos.innerHTML = json.minutos;
            segundos.innerHTML = json.segundos;




            $(function () {
                var chart = window.chart = $('#chartWiFi').data('easyPieChart');
                chart.update(json.WFPRC);
                var chart = window.chart = $('#chartPMEM').data('easyPieChart');
                chart.update(json.PMEM);
                var chart = window.chart = $('#chartram').data('easyPieChart');
                chart.update(json.ram);
                var chart = window.chart = $('#chartcpu').data('easyPieChart');
                chart.update(json.cpu);
            });

        }

        if (pathname == "/indexMed") {

            if (json.WFEstatus != null) {


                let TemperaturaH1 = document.getElementById('TemperaturaH1');
                let TemperaturaH2 = document.getElementById('TemperaturaH2');
                let TemperaturaAmbiente = document.getElementById('TemperaturaAmbiente');
                let TensionGeneral = document.getElementById('TensionGeneral');
                let TensionH1 = document.getElementById('TensionH1');
                let TensionH2 = document.getElementById('TensionH2');
                let magnetismoInterno = document.getElementById('magnetismoInterno');
                let EstadoDeSistema = document.getElementById('EstadoDeSistema');
                let AlarmaIntrusion = document.getElementById('AlarmaIntrusion');
                let AlarmaH1 = document.getElementById('AlarmaH1');
                let AlarmaH2 = document.getElementById('AlarmaH2');
                let EstadoPuerto0 = document.getElementById('EstadoPuerto0');
                let EstadoPuerto1 = document.getElementById('EstadoPuerto1');
                let EstadoPuerto2 = document.getElementById('EstadoPuerto2');



                let dia = document.getElementById('dia');
                let mes = document.getElementById('mes');
                let ano = document.getElementById('ano');
                let horas = document.getElementById('horas');
                let minutos = document.getElementById('minutos');
                let segundos = document.getElementById('segundos');




                TemperaturaH1.innerHTML = json.TemperaturaH1;
                TemperaturaH2.innerHTML = json.TemperaturaH2;
                TemperaturaAmbiente.innerHTML = json.TemperaturaAmbiente;
                TensionGeneral.innerHTML = json.TensionGeneral;
                TensionH1.innerHTML = json.TensionH1;
                TensionH2.innerHTML = json.TensionH2;
                magnetismoInterno.innerHTML = json.magnetismoInterno;
                EstadoDeSistema.innerHTML = json.EstadoDeSistema;
                AlarmaIntrusion.innerHTML = json.AlarmaIntrusion;
                AlarmaH1.innerHTML = json.AlarmaH1;
                AlarmaH2.innerHTML = json.AlarmaH2;
                EstadoPuerto0.innerHTML = json.EstadoPuerto0;
                EstadoPuerto1.innerHTML = json.EstadoPuerto1;
                EstadoPuerto2.innerHTML = json.EstadoPuerto2;
                Corriente1.innerHTML = json.Corriente1;
                Corriente2.innerHTML = json.Corriente2;
                Gas.innerHTML = json.Gas;

                horas.innerHTML = json.horas;
                minutos.innerHTML = json.minutos;
                segundos.innerHTML = json.segundos;
            }

        }

    }
}

//Llamado restablecer
function restablecer() {

    Swal.fire({
        title: 'Restablecer!',
        text: ' ¿Está seguro de restablecer el equipo?',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Si, restablecer',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            window.location = "reconfig";
        } else if (
            result.dismiss === Swal.DismissReason.cancel
        ) {
            history.back();
        }
    })

}