defmodule PhotoBlogWeb.VoteController do
  use PhotoBlogWeb, :controller

  alias PhotoBlog.Votes
  alias PhotoBlog.Votes.Vote

  action_fallback PhotoBlogWeb.FallbackController

  def index(conn, _params) do
    votes = Votes.list_votes()
    render(conn, "index.json", votes: votes)
  end

  def create(conn, %{"vote" => vote_params}) do
    with {:ok, %Vote{} = vote} <- Votes.create_vote(vote_params) do
      conn
      |> put_status(:created)
      |> put_resp_header("location", Routes.vote_path(conn, :show, vote))
      |> render("show.json", vote: vote)
    end
  end

  def show(conn, %{"id" => id}) do
    vote = Votes.get_vote!(id)
    render(conn, "show.json", vote: vote)
  end

  def update(conn, %{"id" => id, "vote" => vote_params}) do
    vote = Votes.get_vote!(id)

    with {:ok, %Vote{} = vote} <- Votes.update_vote(vote, vote_params) do
      render(conn, "show.json", vote: vote)
    end
  end

  def delete(conn, %{"id" => id}) do
    vote = Votes.get_vote!(id)

    with {:ok, %Vote{}} <- Votes.delete_vote(vote) do
      send_resp(conn, :no_content, "")
    end
  end
end
