defmodule PhotoBlog.VotesTest do
  use PhotoBlog.DataCase

  alias PhotoBlog.Votes

  describe "votes" do
    alias PhotoBlog.Votes.Vote

    @valid_attrs %{value: 42}
    @update_attrs %{value: 43}
    @invalid_attrs %{value: nil}

    def vote_fixture(attrs \\ %{}) do
      {:ok, vote} =
        attrs
        |> Enum.into(@valid_attrs)
        |> Votes.create_vote()

      vote
    end

    test "list_votes/0 returns all votes" do
      vote = vote_fixture()
      assert Votes.list_votes() == [vote]
    end

    test "get_vote!/1 returns the vote with given id" do
      vote = vote_fixture()
      assert Votes.get_vote!(vote.id) == vote
    end

    test "create_vote/1 with valid data creates a vote" do
      assert {:ok, %Vote{} = vote} = Votes.create_vote(@valid_attrs)
      assert vote.value == 42
    end

    test "create_vote/1 with invalid data returns error changeset" do
      assert {:error, %Ecto.Changeset{}} = Votes.create_vote(@invalid_attrs)
    end

    test "update_vote/2 with valid data updates the vote" do
      vote = vote_fixture()
      assert {:ok, %Vote{} = vote} = Votes.update_vote(vote, @update_attrs)
      assert vote.value == 43
    end

    test "update_vote/2 with invalid data returns error changeset" do
      vote = vote_fixture()
      assert {:error, %Ecto.Changeset{}} = Votes.update_vote(vote, @invalid_attrs)
      assert vote == Votes.get_vote!(vote.id)
    end

    test "delete_vote/1 deletes the vote" do
      vote = vote_fixture()
      assert {:ok, %Vote{}} = Votes.delete_vote(vote)
      assert_raise Ecto.NoResultsError, fn -> Votes.get_vote!(vote.id) end
    end

    test "change_vote/1 returns a vote changeset" do
      vote = vote_fixture()
      assert %Ecto.Changeset{} = Votes.change_vote(vote)
    end
  end
end
