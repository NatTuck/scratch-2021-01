defmodule PhotoBlog.Repo.Migrations.CreateVotes do
  use Ecto.Migration

  def change do
    create table(:votes) do
      add :value, :integer, null: false
      add :user_id, references(:users, on_delete: :nothing),
        null: false
      add :post_id, references(:posts, on_delete: :nothing),
        null: false

      timestamps()
    end

    create index(:votes, [:post_id, :user_id], unique: true)
    create index(:votes, [:user_id])
    create index(:votes, [:post_id])
  end
end
