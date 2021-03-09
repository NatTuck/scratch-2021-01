defmodule PhotoBlogWeb.VoteView do
  use PhotoBlogWeb, :view
  alias PhotoBlogWeb.VoteView

  def render("index.json", %{votes: votes}) do
    %{data: render_many(votes, VoteView, "vote.json")}
  end

  def render("show.json", %{vote: vote}) do
    %{data: render_one(vote, VoteView, "vote.json")}
  end

  def render("vote.json", %{vote: vote}) do
    post = PhotoBlogWeb.PostView.render_json(
      vote.post)
    %{
      id: vote.id,
      votes: vote.votes,
      post: post,
    }
  end
end
