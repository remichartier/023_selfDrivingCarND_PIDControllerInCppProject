git config --global user.email "remipr.chartier@gmail.com"
git config --global user.name "remichartier"
git remote set-url origin "git@github.com:remichartier/023_selfDrivingCarND_PIDControllerInCppProject.git"
apt install openssh-client
eval "$(ssh-agent -s)"
ssh-add .ssh/20210214_1219_github_id_ed25519