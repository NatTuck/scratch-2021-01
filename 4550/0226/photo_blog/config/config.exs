# This file is responsible for configuring your application
# and its dependencies with the aid of the Mix.Config module.
#
# This configuration file is loaded before any dependency and
# is restricted to this project.

# General application configuration
use Mix.Config

config :photo_blog,
  ecto_repos: [PhotoBlog.Repo]

# Configures the endpoint
config :photo_blog, PhotoBlogWeb.Endpoint,
  url: [host: "localhost"],
  secret_key_base: "7u8saWUIpXDBYtk84fWv+63KG6dAloVcLPRShiPyDCQtpJkihKlAX0llKXXyg10p",
  render_errors: [view: PhotoBlogWeb.ErrorView, accepts: ~w(html json), layout: false],
  pubsub_server: PhotoBlog.PubSub,
  live_view: [signing_salt: "JtWU3ATx"]

# Configures Elixir's Logger
config :logger, :console,
  format: "$time $metadata[$level] $message\n",
  metadata: [:request_id]

# Use Jason for JSON parsing in Phoenix
config :phoenix, :json_library, Jason

# Import environment specific config. This must remain at the bottom
# of this file so it overrides the configuration defined above.
import_config "#{Mix.env()}.exs"
