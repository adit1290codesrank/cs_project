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
            $("#container").removeAttr("hidden");
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
async function find_transactions() {
    Swal.fire({
        title: "Loading...",
        didOpen: async () => {
            Swal.showLoading();
            var json = JSON.stringify([$("#group_id").attr("class")]);
            const response = await fetch('/find_transactions_group', {
                method: "POST",
                body: json
            });
            response.text().then(function (text) {
                obj = JSON.parse(text);
                var container = ""
                if (!obj[0]) {
                    container = container + `<div class="row p-3 text-center"><h2>No transactions as of now.</h2></div>`
                }
                else {
                    for (var i = 0; i < obj.length; i++) {
                        container = container + `<div class="row p-3">
                                    <div class="card card-hover p-3" style="height:20vh">
                                        <div class="card-body position-relative">
                                            <p class="card-text position-absolute top-0 start-0 ps-4"><small class="text-body-secondary">Paid by</small></p>
                                            <h4 class="card-text position-absolute top-50 start-0 translate-middle-y ps-4 pb-4">${obj[i][2]}</h4>
                                            <p class="card-text position-absolute bottom-0 start-0 ps-4"><small class="text-body-secondary">${obj[i][6]}</small></p>
                                            <h4 class="card-text position-absolute top-50 end-0 translate-middle-y pe-4 pb-5">₹${obj[i][1]}</h4>
                                            <a id=${obj[i][0]} class="transaction-button position-absolute bottom-0 end-0 pe-4 pb-2" style="font-size:25px"><i class="bi bi-info-circle-fill"></i></a>
                                        </div>
                                    </div>
                                </div>`
                    }
                }
                $("#container_transactions").append(container);
                $(".transaction-button").bind("click", async function (element) {
                    var json2 = JSON.stringify([$(this).attr('id')]);
                    const response2 = await fetch('/find_transaction', {
                        method: "POST",
                        body: json2
                    });
                    response2.text().then(function (text2) {
                        var obj2 = JSON.parse(text2);
                        if (!obj2[0]) {
                            Swal.fire({
                                icon: "danger",
                                title: "Invaid",
                                text: "Invalid ID was provided, please try again."
                            })
                        }
                        else {
                            var array = obj2[0][3].split(",");
                            var split_img = ""
                            for (var i = 0; i < array.length; i++) {
                                var index = array[i].indexOf(":");
                                var key = array[i].substring(0, index);
                                var value = array[i].substring(index + 1, array[i].length);
                                split_img = split_img + key + '->' + obj2[0][2] + "[label=" + value + "];";
                            }
                            Swal.fire({
                                icon: "info",
                                title: "Transaction Details",
                                text: `${obj2[0][4]}`,
                                footer: `Transaction was made by ${obj2[0][2]} on ${obj2[0][6]}`,
                                imageUrl: "https://quickchart.io/graphviz?format=png&width=500&height=500&graph=digraph{" + split_img + "}",
                                imageWidth: 200,
                                imageHeight: 200,
                                imageAlt: "Custom image"
                            });
                        }
                    });
                });
            });
            var json3 = JSON.stringify([$("#group_id").attr("class")]);
            const response3 = await fetch('/minimum_cash_flow', {
                method: "POST",
                body: json3
            });
            response3.text().then(function (text) {
                var obj3 = JSON.parse(text)
                if (obj3[0]) {
                    $("#cash-flow-body").append(obj3[0]);
                    $("#view_graph").bind("click", function () {
                        Swal.fire({
                            title: "Graph!",
                            text: "Visualization of current transactions",
                            imageUrl: obj3[1],
                            imageWidth: 400,
                            imageHeight: 400,
                            imageAlt: "Custom image"
                        });
                    });
                }
                else {
                    $("#container_cash_flow").append("Everything is settled");
                    $("#view_graph").bind("click", function () {
                        Swal.fire({
                            icon: "warning",
                            title: "Graph!",
                            text: "No transactions?",
                        });
                    });
                }
            });
            Swal.close();
        },
        allowOutsideClick: () => !Swal.isLoading()
    })
}
$(document).ready(async function () {
    validate_login();
    find_transactions();
    $("#add_transaction").bind("click", function () {
        window.location.href = window.location.pathname.replace("group", "add_transaction");
    })
    $("#members_list").append($("#members_hidden").attr("class"));
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