async function validate_login() {
    Swal.fire({
        title: "Loading...",
        didOpen: async () => {
            Swal.showLoading();
            if (localStorage.getItem("login_id")) {
                var json = JSON.stringify([localStorage.getItem("login_id")]);
            }
            else {
                var json = JSON.stringify([""]);
            }
            const response = await fetch('/validate_login', {
                method: "POST",
                body: json
            });
            response.text().then(function (text) {
                var obj = JSON.parse(text);
                if (!obj[0]) window.location.href = "/login";
                $("#welcome_text").html("Welome, " + obj[1]);
                $("#log_in").prop("hidden", true);
                $("#sign_up").prop("hidden", true);
                $("#log_out").removeAttr("hidden");
            });
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
$(document).ready(function () {
    validate_login();
    $("#add").bind("click", function () {
        Swal.fire({
            title: "Loading...",
            didOpen: async () => {
                Swal.showLoading();
                if ($("#username").val() && $("#amount").val() && $("#payment_desc").val() && $("#split_json").val()) {
                    try {
                        var today = new Date();
                        var minutes;
                        if (today.getMinutes() < 10) {
                            minutes = "0" + String(today.getMinutes());
                        }
                        else {
                            minutes = String(today.getMinutes());
                        }
                        var data = [$("#username").val(), $("#amount").val(), $("#payment_desc").val(), $("#split_json").val(), $("#group_id").text(), String(today.getDate()).padStart(2, '0') + '/' + String(today.getMonth() + 1).padStart(2, '0') + ' ' + String(today.getHours()) + ':' + minutes];
                        var json = JSON.stringify(data);
                        const response = await fetch('/add_transaction_post', {
                            method: "POST",
                            body: json
                        });
                        if (response.ok) {
                            Swal.fire({
                                icon: "success",
                                title: "Success",
                                text: "Transaction Added",
                            }).then(function () {
                                window.location.href = window.location.href.replace("add_transaction", "group");
                            });
                        }
                        if (!response.ok) {
                            return Swal.showValidationMessage(`${JSON.stringify(await response.json())}`);
                        }
                    } catch (error) {
                        Swal.showValidationMessage(`Request failed: ${error}`);
                    }
                }
                else {
                    Swal.fire({
                        title: "Information?",
                        text: "All the information are needed",
                        icon: "warning"
                    });
                }
            },
            allowOutsideClick: () => !Swal.isLoading()
        })
    });
    $("#log_out").bind("click", function () {
        Swal.fire({
            title: "Logging out...",
            didOpen: async () => {
                Swal.showLoading();
                var json = JSON.stringify([localStorage.getItem("login_id")]);
                const response = await fetch('/logout', {
                    method: "POST",
                    body: json
                });
                response.text().then(function (text) {
                    var obj = JSON.parse(text);
                    if (obj[0]) {
                        localStorage.setItem("login_id", "");
                        window.location.href = "/login"
                    }
                    return false;
                });
            },
            allowOutsideClick: () => !Swal.isLoading()
        });
        return false;
    });
});