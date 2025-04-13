async function add_member() {
    Swal.fire({
        title: "Loading...",
        didOpen: async () => {
            Swal.showLoading();
            var json = [];
            if (localStorage.getItem("login_id")) {
                json.push(localStorage.getItem("login_id"));
            }
            else {
                json.push("");
            }
            json.push(window.location.href.replace("http://" + window.location.hostname + "/add_member/", ""));
            const response = await fetch('/add_member_post', {
                method: "POST",
                body: JSON.stringify(json)
            });
            response.text().then(function (text) {
                var obj = JSON.parse(text);
                if (obj[0]) {
                    Swal.close();
                    Swal.fire({
                        title: "Success!",
                        text: "You have been added to the group!",
                        icon: "success",
                        showCancelButton: false,
                        confirmButtonColor: "#3085d6",
                        confirmButtonText: "Great!"
                    }).then((result) => {
                        if (result.isConfirmed) {
                            window.location.href = "http://" + window.location.hostname + "/group/" + window.location.href.replace("http://" + window.location.hostname + "/add_member/", "");
                        }
                    });
                }
                else {
                    window.location.href = "http://" + window.location.hostname + "/group/" + window.location.href.replace("http://" + window.location.hostname + "/add_member/", "");
                }
            });

        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
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
                if (!obj[0]) {
                    window.location.href = "/login";
                }
                else {
                    add_member();
                }
            });
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
$(document).ready(function () {
    validate_login();
});