
import $ from 'jquery';

function vote_done(resp, status, _xhr) {
    console.log("updated", status, resp);
    let post = resp.data.post;

    $(".post-card").each((_ii, card) => {
        let post_id = $(card).data('post-id');
        if (post_id == post.id) {
            $(card).find(".post-score")
                   .text(post.score);
        }
    });
}

function send_vote(post_id, votes) {
    let body = {vote: {post_id, votes}};

    /*
    # AJAX stands for
    #  Asynchronous Javascript And XML
    # but usually just really means
    #  Async HTTP request from JavaScript code.
    */

    $.ajax("/api/votes", {
        method: "post",
        dataType: "json",
        contentType: "application/json; charset=UTF-8",
        data: JSON.stringify(body),
        headers: {
            "x-auth": window.userToken,
        },
        success: vote_done,
        error: console.log,
    });
}

function setup() {
    $(".post-card").each((_ii, card) => {
        let id = $(card).data('post-id');

        $(card).find('.upvote').click(() => {
            send_vote(id, 1);
        });
        $(card).find('.downvote').click(() => {
            send_vote(id, -1);
        });
    });
}

$(setup);
