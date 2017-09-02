Welcome

This directory conmtains sample plugins for XrayNotes.

The initial plugins where created in Visual C++ 6.0/

  To write your own, you may use any tool that creates a Win32 DLL. Please see samples for required
entry points.

  The simplest way to create a new plugin is to breate a blank DLL project, and copy the *.c and *.h files from one of the samples. (ex: plugbw) Rename the .c and .h file to the name of the project. In the example case if you create a project caled tumor_scan rename plugbw.c to tumor_scan.c, and plugbw.k to tumor_scan.h

  Naturally, you may need to rename the references to the header files (#include statements) to reflect the new file names.

 After the DLL is compiled, copy it to the XrayNotes/plugins directory. (by default this is at c:\Progra Files\XrayNotes\plugins\)

The sample projects contain a plst build step, that does this automatically. Adjust to taste.

 When Starting XrayNotes, the plugin's short doc string will show up in the plugin menu under the proc menu button. (in lower left [SW] chevron)

Have fun!

