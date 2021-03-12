defmodule PhotoBlogWeb.PostView do
  use PhotoBlogWeb, :view
  alias PhotoBlogWeb.PostView

  def render("index.json", %{posts: posts}) do
    %{data: render_many(posts, PostView, "post.json")}
  end

  def render("show.json", %{post: post}) do
    %{data: render_one(post, PostView, "post.json")}
  end

  def render("post.json", %{post: post}) do
    %{id: post.id,
      body: post.body,
      photo_hash: post.photo_hash}
  end
end
