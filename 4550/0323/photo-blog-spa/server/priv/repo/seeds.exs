# Script for populating the database. You can run it as:
#
#     mix run priv/repo/seeds.exs
#
# Inside the script, you can read and write to any of your
# repositories directly:
#
#     PhotoBlog.Repo.insert!(%PhotoBlog.SomeSchema{})
#
# We recommend using the bang functions (`insert!`, `update!`
# and so on) as they will fail if something goes wrong.

alias PhotoBlog.Repo
alias PhotoBlog.Users.User
alias PhotoBlog.Posts.Post
alias PhotoBlog.Photos

defmodule Inject do
  def photo(name) do
    photos = Application.app_dir(:photo_blog, "priv/photos")
    path = Path.join(photos, name)
    {:ok, hash} = Photos.save_photo(name, path)
    hash
  end

  def user(name, pass) do
    hash = Argon2.hash_pwd_salt(pass)
    Repo.insert!(%User{name: name, password_hash: hash})
  end
end

alice = Inject.user("alice", "test1")
bob = Inject.user("bob", "test2")

moon = Inject.photo("moon.jpg")
nature = Inject.photo("nature.jpg")

p1 = %Post{
  user_id: alice.id,
  photo_hash: moon,
  body: "Alice says Hi!"
}
Repo.insert!(p1)

p2 = %Post{
  user_id: bob.id,
  photo_hash: nature,
  body: "Bob ate Pizza!"
}
Repo.insert!(p2)
