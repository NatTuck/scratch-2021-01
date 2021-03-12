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

alice = Repo.insert!(%User{ name: "alice", password_hash: "" })
bob = Repo.insert!(%User{ name: "bob", password_hash: "" })
