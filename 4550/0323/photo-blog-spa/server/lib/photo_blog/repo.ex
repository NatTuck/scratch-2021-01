defmodule PhotoBlog.Repo do
  use Ecto.Repo,
    otp_app: :photo_blog,
    adapter: Ecto.Adapters.Postgres
end
