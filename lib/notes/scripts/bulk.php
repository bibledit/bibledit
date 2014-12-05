<?php
/*
Copyright (©) 2003-2014 Teus Benschop.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
?>
<h2><?php echo gettext("Notes Bulk Update") ?></h2>
<p class="error">
  <?php echo gettext("This function is powerful.") ?>
  <?php echo gettext("One mouse click can update many notes at once.") ?>
  <?php echo gettext("Changes made here are not easily reverted.") ?>
</p>
<br>
<p><?php echo gettext("Amount of notes to be updated in one go:") ?> <?php echo $this->notescount ?></p>
<br>
<p><a href="bulk.php?subscribe="><?php echo gettext("Subscribe to the notes") ?></a></p>
<p><a href="bulk.php?unsubscribe="><?php echo gettext("Unsubscribe from the notes") ?></a></p>
<p><a href="assign-n.php"><?php echo gettext("Assign the notes to somebody") ?></a></p>
<p><a href="unassign-n.php"><?php echo gettext("Unassign the notes from somebody") ?></a></p>
<p><a href="status-n.php"><?php echo gettext("Change the status of the notes") ?></a></p>
<p><a href="severity-n.php"><?php echo gettext("Change the severity of the notes") ?></a></p>
<p><a href="bible-n.php"><?php echo gettext("Change the Bible of the notes") ?></a></p>
<p><a href="bulk.php?delete="><?php echo gettext("Delete the notes") ?></a></p>
<br>
<p><a href="index.php"><?php echo gettext("Back to notes list") ?></a></p>
