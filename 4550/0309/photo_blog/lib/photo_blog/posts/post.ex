defmodule PhotoBlog.Posts.Post do
  use Ecto.Schema
  import Ecto.Changeset

  schema "posts" do
    field :body, :string
    field :photo_hash, :string

    belongs_to :user, PhotoBlog.Users.User
    has_many :comments, PhotoBlog.Comments.Comment
    has_many :votes, PhotoBlog.Votes.Vote

    field :score, :integer, virtual: true

    timestamps()
  end

  # (%Post{}, %{"body" => "Hello, there"})
  # =>
  # %Post{body: "Hello, there"}

  @doc false
  def changeset(post, attrs) do
    # This is where we do object validation
    post
    |> cast(attrs, [:body, :photo_hash, :user_id])
    |> validate_required([:body, :photo_hash, :user_id])
  end
end
