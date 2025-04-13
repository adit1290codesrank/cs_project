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
        if ($("#group_name").val() && $("#group_desc").val()) {
            Swal.fire({
                title: "Loading...",
                didOpen: async () => {
                    Swal.showLoading();
                    try {
                        var data = [localStorage.getItem("login_id"), $("#group_name").val(), $("#group_desc").val()];
                        if ($("#formFile").prop("files").length != 0) {
                            data.push(true)
                            var file = $("#formFile").prop("files")[0];
                            var reader = new FileReader();
                            reader.readAsBinaryString(file);
                            reader.onload = async function () {
                                data.push(btoa(reader.result));
                                var json = JSON.stringify(data);
                                const response = await fetch('/add_group_post', {
                                    method: "POST",
                                    body: json
                                });
                                if (response.ok) {
                                    Swal.fire({
                                        icon: "success",
                                        title: "Success",
                                        text: "Group created",
                                    }).then(function () {
                                        window.location.href = window.location.href.replace("add_group", "home");
                                    });
                                }
                                if (!response.ok) {
                                    return Swal.showValidationMessage(`${JSON.stringify(await response.json())}`);
                                }
                            };
                        }
                        else {
                            data.push(false);
                            var json = JSON.stringify(data);
                            const response = await fetch('/add_group_post', {
                                method: "POST",
                                body: json
                            });
                            if (response.ok) {
                                Swal.fire({
                                    icon: "success",
                                    title: "Success",
                                    text: "Group created",
                                }).then(function () {
                                    window.location.href = window.location.href.replace("add_group", "home");
                                });
                            }
                            if (!response.ok) {
                                return Swal.showValidationMessage(`${JSON.stringify(await response.json())}`);
                            }
                        }
                    } catch (error) {
                        Swal.showValidationMessage(`Request failed: ${error}`);
                    }
                },
                allowOutsideClick: () => !Swal.isLoading()
            })
        }
        else {
            Swal.fire({
                title: "Information?",
                text: "All the information is needed",
                icon: "warning"
            });
            return false;
        }
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