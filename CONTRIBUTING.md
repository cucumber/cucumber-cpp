## About to create a new Github Issue?

We appreciate that. But before you do, please learn our basic rules:

* This is not a support or discussion forum. If you have a question, please ask it on [The Cukes Google Group](http://groups.google.com/group/cukes).
* Do you have a feature request? Then don't expect it to be implemented unless you or someone else sends a [pull request](https://help.github.com/articles/using-pull-requests).
* Reporting a bug? We need to know what compiler, operating system and architecture (32 or 64 bit) you are using, including versions of all libraries. Bugs with [pull requests](https://help.github.com/articles/using-pull-requests) get fixed quicker. Some bugs may never be fixed.
* You have to tell us how to reproduce a bug. Bonus point for a [pull request](https://help.github.com/articles/using-pull-requests) with a failing test that reproduces the bug.
* Want to paste some code or output? Put \`\`\` on a line above and below your code/output. See [GFM](https://help.github.com/articles/github-flavored-markdown)'s *Fenced Code Blocks* for details.
* We love [pull requests](https://help.github.com/articles/using-pull-requests), but if you don't have a test to go with it we probably won't merge it.

## Contributing

Before you can contribute, you have to be able to build the source and run tests.

### The Github Process

The process for using git/github is similar to the [Github-Flow](http://scottchacon.com/2011/08/31/github-flow.html)

* **Anything** in the master branch is good enough to release
* Working on nontrivial features
    + Create a descriptively named branch off of master
    + Commit to that branch locally and regularly
    + Push your work to the same named branch on the server
    + Regularly rebase this branch from master to keep it up to date
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
3. Tag issues so we can do better triage and assignment.
    + People tend to gravitate towards areas of expertise and tags makes it easier to give a ticket to the right person.
4. Update HISTORY.md
    + When you fix a bug or add a feature
    + Add release dates
5. Subscribe to ticket feeds so you stay in the loop and get a chance to provide feedback on tickets
6. The code standard is the existing code
    + Use the same indentation, spacing, line ending, ASCII for source code, UTF-8 everywhere else
7. Use git diff (or git diff --cached if you have staged) before every commit
    + This helps you avoid committing changes you didn't mean to

