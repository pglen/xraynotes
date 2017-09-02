
XSXps.dll: dlldata.obj XSX_p.obj XSX_i.obj
	link /dll /out:XSXps.dll /def:XSXps.def /entry:DllMain dlldata.obj XSX_p.obj XSX_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del XSXps.dll
	@del XSXps.lib
	@del XSXps.exp
	@del dlldata.obj
	@del XSX_p.obj
	@del XSX_i.obj
