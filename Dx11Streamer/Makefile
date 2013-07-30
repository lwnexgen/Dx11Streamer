build:
	@ssh windows 'C:\Users\Sam\build-and-test.bat' | grep -v "DX11S"

push: commit
	@git push

commit:
	@git commit -a -m "changeset for `date`"

test:
	@ssh windows 'C:\Users\Sam\build-and-test.bat' | grep -e "^DX11S:" | sed -e 's/^DX11S: //g'
