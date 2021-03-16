defmodule PhotoBlog.Repo.Migrations.CreatePosts do
  use Ecto.Migration

  def change do
    create table(:posts) do
      add :body, :text, null: false
      add :photo_hash, :string, null: false
      add :user_id, references(:users, on_delete: :nothing),
        null: false

      timestamps()
    end

    create index(:posts, [:user_id])
  end
end
