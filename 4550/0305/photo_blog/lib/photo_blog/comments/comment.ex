defmodule PhotoBlog.Comments.Comment do
  use Ecto.Schema
  import Ecto.Changeset

  schema "comments" do
    field :body, :string
    field :vote, :integer
    belongs_to :post, PhotoBlog.Posts.Post
    belongs_to :user, PhotoBlog.Users.User

    timestamps()
  end

  @doc false
  def changeset(comment, attrs) do
    comment
    |> cast(attrs, [:body, :vote, :post_id, :user_id])
    |> validate_required([:body, :vote, :post_id, :user_id])
  end
end
