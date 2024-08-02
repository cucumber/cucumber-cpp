## About to create a new Github Issue?

We appreciate that. But before you do, please learn our basic rules:

* Do you have a feature request? Then don't expect it to be implemented unless you or someone else sends a [pull request](https://help.github.com/articles/using-pull-requests).
* Reporting a bug? We need to know what compiler, operating system and architecture (32 or 64 bit) you are using, including versions of all libraries. Bugs with [pull requests](https://help.github.com/articles/using-pull-requests) get fixed quicker. Some bugs may never be fixed.
* You have to tell us how to reproduce a bug. Bonus point for a [pull request](https://help.github.com/articles/using-pull-requests) with a failing test that reproduces the bug.
* Want to paste some code or output? Put \`\`\` on a line above and below your code/output. See [GFM](https://help.github.com/articles/github-flavored-markdown)'s *Fenced Code Blocks* for details.
* We love [pull requests](https://help.github.com/articles/using-pull-requests), but if you don't have a test to go with it we probably won't merge it.

## Contributing

Before you can contribute, you have to be able to build the source and run tests.

### The Github Process

The process for using git/github is similar to the [Github-Flow](http://scottchacon.com/2011/08/31/github-flow.html)

* **Anything** in the main branch is good enough to release
* Working on nontrivial features
    + Create a descriptively named branch off of main
    + Commit to that branch locally and regularly
    + Push your work to the same named branch on the server
    + Regularly rebase this branch from main to keep it up to date
* Open a pull request
    + When you need feedback or help
    + You think the branch is ready for merging (you can use the [hub](https://github.com/defunkt/hub#git-pull-request) command-line tool)
* For any nontrivial change, even if you have the rights to merge the pull request yourself, wait before someone else has reviewed and agreed on the change

Here is an [Example](https://github.com/cucumber/bool/pull/12) of this process in action

#### Tips for good commits

1. Read up on [Github Flavored Markdown](https://help.github.com/articles/github-flavored-markdown)
    + Especially links and syntax highlighting. GFM can be used in tickets as well as commit messages (e.g. put "#4" somewhere in a commit message to link ticket 4 to that commit
2. Close tickets with commits if you can
    + Add "Closes #5, #9" somewhere in the commit message to both link and close. See [Issues 2.0 the Next Generation](https://github.com/blog/831-issues-2-0-the-next-generation) for details.
    + Use [this script](https://gist.github.com/aslakhellesoy/4754009) to compile and view GFM locally
3. Subscribe to ticket feeds so you stay in the loop and get a chance to provide feedback on tickets
4. The code standard is the existing code
    + Use the same indentation, spacing, line ending, ASCII for source code, UTF-8 everywhere else
5. Use git diff (or git diff --cached if you have staged) before every commit
    + This helps you avoid committing changes you didn't mean to

## Maintainers' guide

### Merge a PR

- Verify that:
  - All checks have passed
  - At least one maintainer has approved any non-trivial change, and a
    discussion has occurred for any breaking change
  - The branch does not need rebasing or squashing of commits
- To merge:
  - Follow the command line instructions on GitHub
  - If it is either a new feature or a bugfix, specify the `--no-commit`
    flag when merging and add a line to `CHANGELOG.md` following the
    current convention. Add this file to the changes to be committed and
    commit the merge.
  - Commit message should follow the current convention:
    `Merge #42 'Description of the change usually from the PR description'`

### Do a release

- Release commit (e.g. [fdf8a5c](https://github.com/cucumber/cucumber-cpp/commit/fdf8a5c4ef4c51dfa7ea82077f706414a4c6322d)):
  - Change `CHANGELOG.md` renaming the "In Git" section with the release number and date
  - Commit with message `Update changelog for the X.Y.Z release`
  - Create an annotated tag for this commit named `vX.Y.Z`
- New development branch commit (e.g. [da60995](https://github.com/cucumber/cucumber-cpp/commit/da609956fcd42046e5182c6226acd7e53dd7754e)):
  - Add new "In Git" section to `CHANGELOG.md`
  - Commit with message `Preparing history file for next development release`
- Push commits and tags to main
