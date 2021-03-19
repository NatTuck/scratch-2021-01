defmodule PhotoBlogWeb.Router do
  use PhotoBlogWeb, :router

  pipeline :browser do
    plug :accepts, ["html"]
    plug :fetch_session
    plug :fetch_flash
    plug :protect_from_forgery
    plug :put_secure_browser_headers
  end

  pipeline :api do
    plug :accepts, ["json"]
  end

  scope "/", PhotoBlogWeb do
    pipe_through :browser

    get "/", PageController, :index
    get "/photos/:hash", PageController, :photo
  end

  scope "/api/v1", PhotoBlogWeb do
    pipe_through :api

    resources "/users", UserController, except: [:new, :edit]
    resources "/posts", PostController, except: [:new, :edit]
    resources "/comments", CommentController, except: [:new, :edit]
    resources "/votes", VoteController, except: [:new, :edit]
    resources "/session", SessionController, only: [:create]
  end

  if Mix.env() in [:dev, :test] do
    import Phoenix.LiveDashboard.Router

    scope "/" do
      pipe_through :browser
      live_dashboard "/dashboard", metrics: PhotoBlogWeb.Telemetry
    end
  end
end
