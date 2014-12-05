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
<h2><?php echo gettext("Edit resource") ?> "<?php echo $this->name ?>"</h2>
<h3><?php echo gettext("Introduction") ?></h3>
<p>
  <?php echo gettext("Bibledit-Web displays fragents of external web pages to the user.") ?>
  <?php echo gettext("Bibledit-Web extracts the correct bits from that web page.") ?>
  <?php echo gettext("It passes that bit to the user.") ?>
  <?php echo gettext("This is how it works:") ?>
  <?php echo gettext("Bibledit-Web notices the active passage, for example Genesis 1:1.") ?>
  <?php echo gettext("Bibledit-Web then calls a defined PHP function contained in a resource PHP script.") ?>
  <?php echo gettext("It passes book, chapter and verse to the function.") ?>
  <?php echo gettext("The function fetches a page from the web.") ?>
  <?php echo gettext("The function filters this web page.") ?>
  <?php echo gettext("The script returns the filtered content.") ?>
  <?php echo gettext("Bibledit-Web passes that to the user.") ?>
  <?php echo gettext("This method of serving content saves the user a lot of bandwidth.") ?>
  <?php echo gettext("Bibledit-Web strips a lot of stuff away, and only serves the essentials to the user.") ?>
</p>
<p>
  <?php echo gettext("On this page you can edit and test the script.") ?>
</p>
<p><a href="admin.php">[<?php echo gettext("back") ?>]</a></p>
<form action="edit.php?name=<?php echo $this->name ?>" name="name" method="post">
  <h3><?php echo gettext("Code") ?></h3>
  <p><?php echo gettext("Enter the code of the script.") ?></p>
  <textarea name="code"><?php echo $this->code ?></textarea>
  <h3><?php echo gettext("Save and test") ?></h3>
  <select name="book">
    <?php foreach ($this->books as $offset => $book) { ?>
    <option <?php if ($this->book == $book) echo 'selected="selected"' ?> value="<?php echo $book ?>"><?php echo $this->booknames [$offset] ?></option>
    <?php } ?>
  </select>
  <input type="text" name="chapter" value="<?php echo $this->chapter ?>" />
  :
  <input type="text" name="verse" value="<?php echo $this->verse ?>" />
  -
  <input type="submit" name="save" value=<?php echo gettext("Submit") ?> />
</form>
<br>
<h3><?php echo gettext("Result") ?></h3>
<p><?php echo gettext("Output of running the script:") ?></p>
<textarea><?php echo $this->output ?></textarea>
<br>
<?php if ($this->posted) { ?>
<script type="text/javascript">
$(window).load (function () {
  $("body").animate({ scrollTop: $(document).height() - $(window).height() }, 500);
});
</script>
<?php } ?>
<h3><?php echo gettext("Notes") ?></h3>
<ul>
<li>
  <?php echo gettext("It is possible to make verse references in a Resource clickable in Bibledit-Web while the Resource is being displayed.") ?>
  <?php echo gettext("The code of the script should insert links as follows:") ?>
  &lt;a class="navigate" href="19.90.2"&gt;Ps. 90:2&lt;/a&gt;.
  <?php echo gettext("Bibledit-Web will take care of the rest when it displays the Resource.") ?>
</li>
<li>
  <?php echo gettext("Any changes made to the standard resources that come with Bibledit-Web will be overwritten when upgrading the software.") ?>
</li>
<li>
  <?php echo gettext("The script will be saved to file") ?>
  <code><?php echo $this->file; ?></code>.
  <?php echo gettext("The versification system and the verse mapping can be set in file") ?>
  <code><?php echo $this->metafile; ?></code>.
  <?php echo gettext("The default resource files provide examples.") ?>
</li>
</ul>
