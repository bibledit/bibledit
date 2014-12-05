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
<h2><?php echo gettext("Users") ?></h2>
<table>
  <thead>
    <tr>
      <td class="center"><?php echo gettext("Username") ?></td>
      <td></td>
      <td class="center"><?php echo gettext("Role") ?></td>
      <td></td>
      <td class="center"><?php echo gettext("Email") ?></td>
      <td></td>
      <td class="center"><?php echo gettext("Bibles") ?></td>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($this->usernames as $offset => $username) { ?>
    <tr>
      <td><a href="?user=<?php echo $username ?>&delete">✗</a> <?php echo $username ?></td>
      <td>│</td>
      <td><a href="?user=<?php echo $username ?>&level"><?php echo $this->levels[$offset] ?></a></td>
      <td>│</td>
      <td><a href="?user=<?php echo $username ?>&email"><?php echo $this->emails[$offset] ?></a></td>
      <td>│</td>
      <td>
        <?php foreach ($this->bibles [$offset] as $bible) { ?>
          <a href="?user=<?php echo $username ?>&removebible=<?php echo $bible['bible'] ?>">✗</a>
          <a href="../bible/settings.php?bible=<?php echo $bible['bible'] ?>" target="_blank"><?php echo $bible['bible'] ?></a>
          <?php if ($bible['writer']) { ?>
            <a href="?user=<?php echo $username ?>&readonlytoggle=<?php echo $bible['bible'] ?>">
            <?php if ($bible['readonly']) echo "☐"; else echo "☑"; echo "✍"; ?>
            </a>
          <?php } ?>
          |
        <?php } ?>
        <a href="?user=<?php echo $username ?>&addbible=">➕</a>
      </td>
    </tr>
    <?php } ?>
  </tbody>
</table>
<ul>
  <li>
    <?php echo gettext("This lists the accounts of the users who have access to Bibles you have acccess to, and have roles you are responsible for.") ?>
    <?php echo gettext("It also lists users without access to any Bibles.") ?>
  </li>
  <li><?php echo gettext("Click ✗ before a username to delete the account.") ?></li>
  <li><?php echo gettext("Click a user's role to change it.") ?></li>
  <li><?php echo gettext("Click a user's email address to change it.") ?></li>
  <li>
    <a href="?new="><?php echo gettext("You can create a new user.") ?></a>
    <?php echo gettext('The password will be the same as the username.') ?>
    <?php echo gettext('Normally users register their own account.') ?>
  </li>
  <li>
    <?php echo gettext('Bible translation teams.') ?>
    <ul>
      <li><?php echo gettext('Click ➕  to make a user a member of a Bible translation team.') ?></li>
      <li><?php echo gettext('Click ✗ before a Bible to remove that member from the team.') ?></li>
      <li>
        <?php echo gettext('Some roles have write access to Bibles.') ?>
        <?php echo gettext('A symbol ✍ appears.') ?>
        <?php echo gettext('Uncheck to remove write access to that Bible.') ?>
      </li>
      <li><?php echo gettext('You can only see Bibles you have access to yourself.') ?></li>
      <li><?php echo gettext('If no user on the whole site has been granted access to any Bible, then all users have access to all Bibles.') ?></li>
    </ul>
  </li>
</ul>
