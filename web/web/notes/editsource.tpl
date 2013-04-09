<h1>{t}Edit Note Source{/t}</h1>
<p class="error">{$error}</p>
<p class="success">{$success}</p>
<p>{t}You can edit the source of a Consultation Note.{/t}</p>
<p>{t}Each Consultation Notes has an identifier.{/t}</p>
<p>{t}The identifier of a Consultation Note can be seen through the Actions link of that note.{/t}</p>
<p>{t}Example:{/t}</p>
<p>{t}Identifier:{/t} 943430030.</p>
<br>
<p>{t}Enter the identifier of the Consultation Note to be edited and press Submit.{/t}</p>
<form action="editsource.php" name="noteid" method="post">
  <p>
  <input type="text" name="identifier" maxlength="15" value="{$identifier}" />
  <input type="submit" name="submit" value={t}Submit{/t} />
  </p>
</form>
<br>
<form action="editsource.php?identifier={$identifier}" name="notetext" method="post">
  <p><textarea name="data">{$data}</textarea></p>
  <p><input type="submit" name="save" value={t}Save{/t} /></p>
</form>
