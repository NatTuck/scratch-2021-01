defmodule AwwTitles do
  def reddit_list(sub) do
    url = "https://www.reddit.com/r/#{sub}.json"
    resp = HTTPoison.get!(url)
    data = Jason.decode!(resp.body)
    data["data"]["children"]
  end

  def get_titles(sub) do
    xs = reddit_list(sub)
    Enum.map xs, fn x ->
      x["data"]["title"]
    end
  end
end
