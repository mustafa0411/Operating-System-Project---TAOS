# Configuring Git and SSH

We will use Git to interact with GitHub for managing source code. Git relies on
SSH for secure communications with remote servers, so we'll set that up
first.

## Configuring SSH

### Generating a key

GitHub requires SSH to be authenticated using a key. If you already have an
SSH key, you can go to the following step. Otherwise, generate a new key by
running:

    ssh-keygen

You are strongly suggested to use the default file name. If you specify a
different file name for your key, you are on your own for configuring Git and
SSH to use that key.

You can use a passphrase on your key for additional security, or leave it
blank. Just make sure it's something you can remember if you do set a
passphrase.

Pay particular attention the line of output beginning with `Your public key
has been saved in`. You will need the file name that comes after in the next
step.

### Adding your public key to GitHub

Display the contents of your public key (if you used the default file name, the
following should work):

    cat ~/.ssh/id_rsa.pub

In a browser, login to GitHub. In the upper right, click your user icon, then
select Settings from the drop-down menu. On the left side of the screen,
select "SSH and GPG keys". Click the "New SSH key" button. Copy your public
key (the output of the `cat` command) and paste it in the "Key" box. Finally,
click "Add SSH key".

## Configuring Git

With SSH set up, Git should be ready to communicate with GitHub. However, you
still need to configure Git with your name and email address to make sure you
get proper credit for your contributions. Simply run:

    git config --global user.name "Your Name"
    git config --global user.email "Your Email Address"

Obviously, replace *Your Name* and *Your Email Address* with your actual name
and email address, but make sure you leave the quotes.
