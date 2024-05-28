Function Show-MenuSelect()
{

    Param(
        [Parameter(Mandatory = $True)][String]$MenuTitle,
        [Parameter(Mandatory = $True)][String]$MenuBanner,
        [Parameter(Mandatory = $True)][array]$MenuOptions
    )

    $MaxValue = $MenuOptions.count - 1
    $Selection = 0
    $EnterPressed = $False

    Clear-Host

    While ($EnterPressed -eq $False)
    {
        if ($MenuBanner.Length -ne 1)
        {
            Write-Host "$MenuBanner"
        }
        Write-Host "$MenuTitle"

        For ($i = 0; $i -le $MaxValue; $i++) {

            If ($i -eq $Selection)
            {
                Write-Host -BackgroundColor DarkGray -ForegroundColor White "[ $( $MenuOptions[$i] ) ]"
            }
            Else
            {
                Write-Host "  $( $MenuOptions[$i] )  "
            }

        }

        $KeyInput = $host.ui.rawui.readkey("NoEcho,IncludeKeyDown").virtualkeycode

        Switch ($KeyInput)
        {
            13 {
                $EnterPressed = $True
                Return $Selection
                Clear-Host
                break
            }

            38 {
                If ($Selection -eq 0)
                {
                    $Selection = $MaxValue
                }
                Else
                {
                    $Selection -= 1
                }
                Clear-Host
                break
            }

            40 {
                If ($Selection -eq $MaxValue)
                {
                    $Selection = 0
                }
                Else
                {
                    $Selection += 1
                }
                Clear-Host
                break
            }
            Default {
                Clear-Host
            }
        }
    }
}

$banner = @"
  ___  ______  _____  _   _   _    _ _____ _   _
 / _ \ | ___ \/  __ \| | | | | |  | |_   _| \ | |
/ /_\ \| |_/ /| /  \/| |_| | | |  | | | | |  \| |
|  _  ||    / | |    |  _  | | |/\| | | | | . ` |
| | | || |\ \ | \__/\| | | | \  /\  /_| |_| |\  |
\_| |_/\_| \_| \____/\_| |_/  \/  \/ \___/\_| \_/


****************************************************************
* Georg Fischer IT Utilities                                   *
* Copyright of Colin Heggli $( (Get-Date).Year )                               *
* https://colin.heggli.dev                                     *
* https://github.com/M4rshe1                                   *
****************************************************************


"@

function Get-CDROMDriveLetter
{
    $CDROM = ""
    foreach ($driveLetter in 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z')
    {
        $_
        if (Test-Path -Path "$( $driveLetter ):\sources\install.wim")
        {
            $CDROM = "$( $driveLetter ):"
            break
        }
    }
    if ($CDROM -eq "")
    {
        Read-Host "ERROR: Install CD not found."
        exit
    }
    return $CDROM
}

function Select-InstallDisk
{

    @"
list disk
exit
"@ | Out-File -FilePath "$env:temp\diskpart.txt"

    $disks = @()
    $diskOptions = @()
    (diskpart /s "$env:temp\diskpart.txt") -split "`n`r" | ForEach-Object {
        if ($_ -match "(\d+)\s+Online\s+(\d+)\s+GB\s+(\d+)\s+")
        {
            $diskOptions += "Disk $( $matches[1] ) - $( $matches[2] ) GB ($( $matches[3] ) GB free)"
            $disks += [PSCustomObject]@{
                disk = $matches[1]
                size = $matches[2]
                free = $matches[3]
            }
        }
    }

    $selectedDisk = Show-MenuSelect -MenuTitle "Select a disk to install Windows" -MenuBanner "$banner" -MenuOptions $diskOptions
    return $disks[$selectedDisk]
}


function select-WinVersion
{
    $CDROM = Get-CDROMDriveLetter
    $editions = @()

    $verison = DISM /Get-ImageInfo /imagefile:$CDROM\sources\install.wim

    $IndexName = @()
    $verison = $verison -split "`n`r"
    for ($i = 0; $i -lt $verison.Length; $i++) {
        if ($verison[$i] -match "Index (\d+)")
        {
            $editions += "Index $( $matches[1] ) - $( $verison[$i + 1] -replace 'Name : ', '' )"
            $IndexName += [PSCustomObject]@{
                Index = $matches[1]
                Name = $verison[$i + 1] -replace 'Name : ', ''
            }
        }
    }

    $selectedEdition = Show-MenuSelect -MenuTitle "Select an edition to install" -MenuBanner "$banner" -MenuOptions $editions
    return $IndexName[$selectedEdition]
}



function install-Windows
{
    param (
        [int]$index,
        [string]$disk,
        [string]$CDROM
    )

    $diskpartCommands = @"
select disk $DISK
clean
convert gpt
create partition efi size=100
format quick fs=fat32 label="EFI"
assign letter=B:
create partition primary
format quick fs=ntfs label="Windows"
assign letter=A:
exit
"@ | Out-File -FilePath "$env:temp\diskpart_commands.txt"

    diskpart /s "$env:temp\diskpart_commands.txt"

    DISM /Apply-Image /imagefile:$CDROM\sources\install.wim /Index:$index /ApplyDir:A:
    bcdboot A:\Windows /s B: /f ALL
    Remove-Item "$env:temp\diskpart.txt"
    Remove-Item "$env:temp\diskpart_commands.txt"

}

function Add-DefaultUser
{
    Write-Host "Do you want to add a default user? (Y/N)"
    $addUser = Read-Host ">> "
    if ($addUser.ToLower() -eq "y")
    {
        While ($true)
        {
            Write-Host "Enter username"
            $username = Read-Host ">> "
            if ($username.Length -eq 0)
            {
                Write-Host "Username cannot be empty."
            }
            else
            {
                break
            }
        }
        While ($true)
        {
            Write-Host "Enter password"
            $password = Read-Host ">> "
            if ($password.Length -eq 0)
            {
                Write-Host "Password cannot be empty."
            }
            else
            {
                break
            }
        }
    }
}



function start-main
{
    While ($true)
    {
        Clear-Host
        $disk = Select-InstallDisk
        Clear-Host
        $version = select-WinVersion

        Clear-Host
        Write-Host $banner
        Write-Host "Please confirm the following settings:"
        Write-Host "Disk: $( $disk.disk ) - $( $disk.size ) GB ($( $disk.free ) GB free)"
        Write-Host "Edition: $( $version.Index ) - $( $version.Name )"

        Write-Host "Do you want to continue? (Y/N)"
        $continue = Read-Host ">> "
        if ($continue.ToLower() -eq "y")
        {
            break
        }
    }
    Add-DefaultUser
}

start-main