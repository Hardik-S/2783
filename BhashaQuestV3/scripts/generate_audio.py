#!/usr/bin/env python3
"""
Generate TTS audio files for BhashaQuest number exercises
Uses gTTS for text-to-speech generation

Usage:
    pip install gtts
    python generate_audio.py
"""

import os
from pathlib import Path

try:
    from gtts import gTTS
    GTTS_AVAILABLE = True
except ImportError:
    GTTS_AVAILABLE = False
    print("⚠️  gTTS not installed. Run: pip install gtts")

# Audio mapping: filename -> (text, language_code, description)
AUDIO_FILES = {
    # Nepali numbers (using Hindi TTS as proxy - similar pronunciation)
    "ek.mp3": ("ek", "hi", "Nepali: one"),
    "panch.mp3": ("paanch", "hi", "Nepali: five"),
    "counting.mp3": ("ek dui teen", "hi", "Nepali: 1-2-3 counting"),
    "das.mp3": ("das", "hi", "Nepali: ten"),
    "bees.mp3": ("bees", "hi", "Nepali: twenty"),

    # Kannada numbers (using Kannada TTS - native script)
    "ondu.mp3": ("ಒಂದು", "kn", "Kannada: one"),
    "aidu.mp3": ("ಐದು", "kn", "Kannada: five"),
    "kan-counting.mp3": ("ಒಂದು ಎರಡು ಮೂರು", "kn", "Kannada: 1-2-3 counting"),
    "hattu.mp3": ("ಹತ್ತು", "kn", "Kannada: ten"),
    "ippatthu.mp3": ("ಇಪ್ಪತ್ತು", "kn", "Kannada: twenty"),

    # Success/error feedback sounds
    "success.mp3": ("Correct! Well done!", "en", "Success feedback"),
    "error.mp3": ("Try again", "en", "Error feedback"),
}


def generate_audio_files(output_dir: str = None):
    """Generate all audio files using gTTS"""

    if not GTTS_AVAILABLE:
        print("\n❌ Cannot generate audio: gTTS not installed")
        print("   Install it with: pip install gtts")
        return False

    # Default to ../assets/audio relative to this script
    if output_dir is None:
        script_dir = Path(__file__).parent
        output_dir = script_dir.parent / "assets" / "audio"
    else:
        output_dir = Path(output_dir)

    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"🎵 Generating {len(AUDIO_FILES)} audio files...")
    print(f"📁 Output directory: {output_dir.absolute()}\n")

    success_count = 0
    error_count = 0

    for filename, (text, lang, description) in AUDIO_FILES.items():
        filepath = output_dir / filename

        try:
            print(f"  🔊 {filename:20} | {description:25} | ", end="")

            # Generate TTS audio
            tts = gTTS(text=text, lang=lang, slow=False)
            tts.save(str(filepath))

            # Verify file was created
            if filepath.exists():
                file_size = filepath.stat().st_size
                print(f"✅ ({file_size:,} bytes)")
                success_count += 1
            else:
                print(f"❌ File not created")
                error_count += 1

        except Exception as e:
            print(f"❌ Error: {e}")
            error_count += 1

    print(f"\n{'='*70}")
    print(f"✅ Successfully generated: {success_count}/{len(AUDIO_FILES)} files")
    if error_count > 0:
        print(f"❌ Errors: {error_count} files")
    print(f"📂 Location: {output_dir.absolute()}")
    print(f"{'='*70}\n")

    return error_count == 0


def list_audio_files(output_dir: str = None):
    """List existing audio files"""

    if output_dir is None:
        script_dir = Path(__file__).parent
        output_dir = script_dir.parent / "assets" / "audio"
    else:
        output_dir = Path(output_dir)

    if not output_dir.exists():
        print(f"❌ Directory does not exist: {output_dir}")
        return

    print(f"\n📂 Audio files in: {output_dir.absolute()}\n")

    mp3_files = sorted(output_dir.glob("*.mp3"))

    if not mp3_files:
        print("  (No MP3 files found)")
    else:
        for mp3_file in mp3_files:
            file_size = mp3_file.stat().st_size
            # Get description from AUDIO_FILES if available
            desc = AUDIO_FILES.get(mp3_file.name, ("", "", ""))[2]
            if desc:
                print(f"  🎵 {mp3_file.name:20} | {file_size:>7,} bytes | {desc}")
            else:
                print(f"  🎵 {mp3_file.name:20} | {file_size:>7,} bytes")

    print(f"\n  Total: {len(mp3_files)} files\n")


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1 and sys.argv[1] == "--list":
        list_audio_files()
    else:
        print("\n" + "="*70)
        print("BhashaQuest Audio Generator".center(70))
        print("="*70 + "\n")

        if generate_audio_files():
            print("✅ Audio generation complete!")
            print("\n💡 Test playback in BhashaQuest app:")
            print("   1. Build and run the app")
            print("   2. Start a 'Numbers & Counting' lesson")
            print("   3. Click the 🔊 Listen button")
        else:
            print("❌ Audio generation failed. Check errors above.")
            sys.exit(1)
