function copy(element) {
    console.log("http://" + window.location.hostname + $(element).attr('src'));
    navigator.clipboard.writeText("http://" + window.location.hostname + $(element).attr('src'));
    Swal.fire({
        icon: "success",
        title: "Copied!",
        text: "Link has been copied to your clipboard, share with others to add!",
        footer: '<a href="' + 'http://' + window.location.hostname + $(element).attr('src') + '">Copy this link if didnt work.</a>'
    });
};
var obj;
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
async function find_groups() {
    Swal.fire({
        title: "Loading...",
        didOpen: async () => {
            Swal.showLoading();
            var json = JSON.stringify([localStorage.getItem("login_id")]);
            const response = await fetch('/find_groups', {
                method: "POST",
                body: json
            });
            response.text().then(function (text) {
                obj = JSON.parse(text);
                var container = ""
                if (!obj[0]) {
                    container = container + `<div class="row p-3 text-center"><h2>You are in no groups as of now.</h2></div>`
                }
                else {
                    for (var i = 0; i < obj.length; i++) {
                        container = container + `<div class="row p-3"><div class="card" ><h5 class="card-header">` + obj[i][3] + `</h5><div class="card-body"><div class="row g-0"><div class="col-md-4"><img src="` + obj[i][5] + `" class="img rounded-start" style="height:200px" alt="..."></div><div class="col-md-8 position-relative"><h5 class="card-title position-absolute top-0 start-50 translate-middle-x">Owner : ` + obj[i][1] + `</h5><p class="card-text position-absolute top-50 start-50 translate-middle">` + obj[i][4] + `</p><a onclick="copy(this);" src="/add_member/` + obj[i][0] + `" class="btn btn-primary position-absolute top-0 end-0"><i class="bi bi-person-add"></i></a><a href="/group/` + obj[i][0] + `" class="btn btn-primary position-absolute bottom-0 end-0">Group Description -></a></div></div></div></div></div>`;
                    }
                }
                $("#container").append(container);
            });
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
$(document).ready(function () {
    validate_login();
    find_groups();
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
        })
        return false;
    })
});