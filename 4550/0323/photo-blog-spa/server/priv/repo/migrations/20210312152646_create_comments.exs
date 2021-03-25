defmodule PhotoBlog.Repo.Migrations.CreateComments do
  use Ecto.Migration

  def change do
    create table(:comments) do
      add :body, :text, null: false
      add :user_id, references(:users, on_delete: :nothing),
        null: false
      add :post_id, references(:posts, on_delete: :nothing),
        null: false

      timestamps()
    end

    create index(:comments, [:user_id])
    create index(:comments, [:post_id])
  end
end
