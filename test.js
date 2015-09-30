$(document).ready(function() {


	var refresh = 1000;
    var updateStatus = function() {

        command("status", "", function(data) {
        	console.log(data);
            $('#status').text(data.status);
            setTimeout(updateStatus, refresh);
        });


    };


    setTimeout(updateStatus, refresh);



    var command = function(name, param, cb) {
        $.ajax({
            type: "POST",
            url: "/driver",
            data: JSON.stringify({
                "cmd": name,
                "param": param
            }),
            success: function(data) {
            	console.log(data);
                cb(data);
            },
            error: function(xhr, ajaxOptions, thrownError) {
            	console.log(xhr);
            },
            dataType: 'json'
        });
    };
    $('#start').click(function() {
        var steps = $('#steps').val();
        var dir = $('#direction').val();

        var success = function() {
            alert("Motor started");
        };

        command("run", dir + " " + steps, success);
    });

    $('#stop').click(function() {
        command("stop", [], function() {
            alert("Motor stopped");
        });
    });


});
