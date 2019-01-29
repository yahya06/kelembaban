// inisialisasi Firebase
var config = {
	apiKey: "AIzaSyBz5JEB7hewa9-_wufMSHAcAWW2k3gNUDY",
	authDomain: "kelembaban-aya.firebaseapp.com",
	databaseURL: "https://kelembaban-aya.firebaseio.com",
	projectId: "kelembaban-aya",
	storageBucket: "kelembaban-aya.appspot.com",
	messagingSenderId: "309101464289"
	};
firebase.initializeApp(config);

$(document).ready(function(){
	//mengambil nilai(value) pada child sinyal
	var ref = firebase.database().ref("sinyal");
	ref.on("value", function(snapshot) {
	   console.log(snapshot.val());
	   $("#setSinyal").text(snapshot.val()+" dBm"); 
	}, function (error) {
	   console.log("Error: " + error.code);
	});
	//mengambil nilai(value) pada child persenKelembapan
	var ref = firebase.database().ref("persenKelembapan");
	ref.on("value", function(snapshot) {
	   console.log(snapshot.val());
	   $("#setKelembapan").text(snapshot.val()+" %"); 
	}, function (error) {
	   console.log("Error: " + error.code);
	});

	//pemberian text pada #setIndikator
	var ref = firebase.database().ref("indikator");
	ref.on("value", function(snapshot) {
	   console.log(snapshot.val());
	   if (snapshot.val() == 1) {
	    	$("#setIndikator").text("Tanah Kering");
	    	$("#setIndikator").addClass("bg-danger");
	    } else if (snapshot.val() == 2) {
	    	$("#setIndikator").text("Tanah Normal");
	    	$("#setIndikator").addClass("bg-success");
	    } else if (snapshot.val() == 3) {
	    	$("#setIndikator").text("Tanah Basah");
	    	$("#setIndikator").addClass("bg-primary");
	    }
	}, function (error) {
	   console.log("Error: " + error.code);
	});
});

//membuat chart
google.charts.load('current', {packages: ['gauge']});
google.charts.setOnLoadCallback(drawChart);

function drawChart() {
	// mendefinisikan chart yg akan di buat
	//mengambil nilai(value) pada child persenKelembapan
	firebase.database().ref('persenKelembapan').on('value', function(snap) {
		var sensor = snap.val();
		var mydata = google.visualization.arrayToDataTable([
			['Label', 'Value'], ['Kelembapan',  sensor ]
		]);
		var myoptions = {width: 600, height: 320, redFrom: 0, redTo: 20,
			yellowFrom: 20, yellowTo: 75, greenFrom: 75, greenTo: 100, minorTicks: 20};

		//memvisualisasikan chart dengan 'Id' #chart_div
		chart = new google.visualization.Gauge(document.getElementById('chart_div'));
		chart.draw(mydata, myoptions);
	});
}


//otomatisasi logout
var timeoutWarning = 840000;
var timeoutNow = 60000;
var logoutUrl = 'index.html';
function StartWarningTimer(){
	warningTimer = setTimerout("IdleWarning()", timeoutWarning);
}
function resetTimeOutTimer(){
	clearTimerout(timeoutTimer);
	StartWarningTimer();
	$('#timeout').dialog('close')
}
function IdleWarning() {
    alert("Warning, your page will redirected to login page. Due to not move your mouse within the page in 15 minutes.");
}

function IdleTimeout() {
    window.location = logoutUrl;
}