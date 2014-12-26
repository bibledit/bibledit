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
<h1><?php echo gettext("Import USFM") ?></h1>
<p class="success"><?php echo $this->success_message ?></p>
<p class="error"><?php echo $this->error_message ?></p>
<p><?php echo gettext("Bible") ?>: <a href="settings.php?bible=<?php echo $this->bible ?>"><?php echo $this->bible ?></a></p>
<p><a href="../journal/index.php" target="_blank"><?php echo gettext("Journal") ?></a></p>
<p>
  <?php echo gettext("This helps you with importing Unified Standard Format Markers data.") ?>
  <?php echo gettext("More informatoin about this format:") ?>
  <a href="http://paratext.org/about/usfm" target="_blank"><?php echo gettext("Source 1: Paratext.") ?></a>
  <a href="http://ubs-icap.org/chm/usfm/" target="_blank"><?php echo gettext("Source 2: UBS ICAP.") ?></a>
</p>
<p>
  <?php echo gettext("The data to be imported will overwrite similar data already in the Bible.") ?>
  <?php echo gettext("If you import for example Genesis chapter 1, it will overwrite Genesis 1 if that chapter already was in the Bible.") ?>
  <?php echo gettext("It will not overwrite anything else.") ?>
  <?php echo gettext("If Genesis 1 was not yet there, it will be created.") ?>
  <?php echo gettext("You can import one chapter, or several chapters, or one book, or several books, all in one go.") ?>
  <?php echo gettext("Bibledit will recognize books through the \\id markup and chapters through the \\c markup.") ?>
  <?php echo gettext("The text to be imported should be in Unicode UTF-8 format.") ?>
</p>
<br>
<p>
  <?php echo gettext("To import the data, paste the data into the box below, and then press the Submit button.") ?>
</p>
<form action="?bible=<?php echo $this->bible ?>" name="form" method="post">
  <p><textarea name="data"></textarea></p>
  <p><input type="submit" name="submit" value=<?php echo gettext("Submit") ?> /></p>
</form>
<br>
<p><?php echo gettext("You can also upload a file.") ?></p>
<p>
  <?php echo gettext("The configuration of the server on which Bibledit limits limits the size of the file to be uploaded.") ?>
  <?php echo gettext("The size of the file should not exceed any of the two limits given below:") ?>
</p>
<p><?php echo gettext("Maximum size of a file to be posted:") ?> <?php echo $this->post_max_size ?></p>
<p><?php echo gettext("Maximum size of a file to be uploaded:") ?> <?php echo $this->upload_max_filesize ?></p>
<p><?php echo gettext("The file to be uploaded can be a USFM file or a compressed archive with USFM files.") ?></p>
<form enctype="multipart/form-data" action="?bible=<?php echo $this->bible ?>" method="POST">
  <input name="data" type="file" />
  <input type="submit" name="upload" value="<?php echo gettext("Upload") ?>" />
</form>
<p>
  <?php echo gettext("Uploading may take some time.") ?>
  <?php echo gettext("It depends on the speed of your network and on the size of the file.") ?>
</p>
<p><?php echo gettext("If uploading fails, it may be because the file is too large, or the network too slow.") ?></p>
