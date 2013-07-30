build:
	@ssh windows 'C:\Users\Sam\build-and-test.bat'

push:
	@git commit -a -m "changeset for `date +%Y%m%d%s`"

test:
	@ssh windows 'C:\Users\Sam\build-and-test.bat' | grep -e "^DX11S:" | sed -e 's/^DX11S: //g'
