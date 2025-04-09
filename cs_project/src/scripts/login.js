async function validate_login() {
    Swal.fire({
        title: "Loading...",
        didOpen: async () => {
            Swal.showLoading();
            if (localStorage.getItem("login_id")) {
                var json = JSON.stringify([localStorage.getItem("login_id")]);
                const response = await fetch('/validate_login', {
                    method: "POST",
                    body: json
                });
                response.text().then(function (text) {
                    var obj = JSON.parse(text);
                    if (obj[0]) window.location.href = "/home";
                });
            }
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
$(document).ready(function () {
    validate_login();
    $("#login").bind("click", function () {
        if ($("#logusername").val() && $("#logpass").val()) {
            Swal.fire({
                title: "Loading...",
                didOpen: async () => {
                    Swal.showLoading();
                    try {
                        var creds = [$("#logusername").val(), $("#logpass").val()];
                        var json = JSON.stringify(creds);
                        const response = await fetch('/login_user', {
                            method: "POST",
                            body: json
                        });
                        response.text().then(function (text) {
                            const obj = JSON.parse(text);
                            if (obj[0]) {
                                localStorage.setItem("login_id", obj[1]);
                                window.location.href = "/home";
                            }
                            if (!obj[0]) Swal.fire("Incorrect credentials");
                        })
                        if (!response.ok) {
                            return Swal.showValidationMessage(`${JSON.stringify(await response.json())}`);
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
                title: "Username and Password?",
                text: "Both username and password are needed",
                icon: "warning"
            });
            return false;
        }
    });
    $("#signup").bind("click", function () {
        if ($("#signusername").val() && $("#signpass").val() && ($("#signpass").val() == $("#signconfirm").val())) {
            Swal.fire({
                title: "Loading...",
                didOpen: async () => {
                    Swal.showLoading();
                    try {
                        var creds = [$("#signusername").val(), $("#signpass").val()];
                        var json = JSON.stringify(creds);
                        console.log(json);
                        const response = await fetch('/signup', {
                            method: "POST",
                            body: json
                        });
                        response.text().then(function (text) {
                            const obj = JSON.parse(text);
                            if (obj[0]) {
                                Swal.fire({
                                    title: "Success!",
                                    text: "Sign up successfull!",
                                    icon: "success"
                                }).then(function () {
                                    window.location.href = "/login";
                                });
                            }
                            if (!obj[0]) Swal.fire({
                                title: "Already Exists....",
                                text: "Username already exists, try something else",
                                icon: "warning"
                            });
                        })
                        if (!response.ok) {
                            return Swal.showValidationMessage(`${JSON.stringify(await response.json())}`);
                        }
                    } catch (error) {
                        Swal.showValidationMessage(`Request failed: ${error}`);
                    }
                },
                allowOutsideClick: () => !Swal.isLoading()
            })
        }
        else if (!$("#signusername").val() || !$("#signpass").val()) {
            Swal.fire({
                title: "Username and Password?",
                text: "Both username and password are needed",
                icon: "warning"
            });
            return false;
        }
        else {
            Swal.fire({
                title: "Need to match?",
                text: "Passwords not matching",
                icon: "warning"
            });
            return false;
        }
    });
});