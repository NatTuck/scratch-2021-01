// We need to import the CSS so that webpack will load it.
// The MiniCssExtractPlugin is used to separate it out into
// its own CSS file.
import "../css/app.scss";

// webpack automatically bundles all modules in your
// entry points. Those entry points can be configured
// in "webpack.config.js".
//
// Import deps with the dep name or local files with a relative path, for example:
//
//     import {Socket} from "phoenix"
//     import socket from "./socket"
//
import "phoenix_html";
import $ from 'jquery';

import { get_channel } from "./socket";

function show_resp({text}) {
    $('#output').text(text);
}

$('#btn').click(() => {
    let chan = get_channel();
    let msg = $("#input").val();
    chan.push("echo", {text: msg})
        .receive("ok", show_resp);
});
