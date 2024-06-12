# How to get started?
## What do i need?
- Docker
- Devcontainer extension for VSCode (ar any other editor that you use that can run devcontainers)

# Setup
1. Clone this Repository
2. Open the Repo in your Code editor (preferably VSCode)
3. In VSCode you will either see a little popup window on the bottom right that says "Reopen in container" or click in the little blue square on the bottom left of the VSCode window and click "Reopen in Container"
4. run `make` (this will run the game)
5. Open localhost in your browser to see the visualizer (if you open it throught VSCode and it doesn't show the visualizer then just remove the port 4242 from the URL)
6. go to src/main.c to the ft_user_loop function and start creating your bot

# Useful Commands
- Run a game: `make` or `make run`
- Stop a game: `make stop`
- Update CORE: `make update`

# Reminder
Because you cloned repository you have to change the origin to your own repo if you want to push the code somewhere to git
