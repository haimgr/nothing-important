package game.entities


/**
  * Created by Haim Greenstein on 07 sep. 2016. (c) All rights reserved.
  * ----
  * We must know the Class object of value before OurFactory is instantiated, but we want also to
  * derive the Value trait. So in scala we can declare the value's Class-object out of the factory,
  * and extend the OurFactory.Value trait, and give the new class to the factory. This is possible
  * because the factory is a class, and the value is a class, the static part of a class which
  * includes the Class object, is instantiated before the objects of classes are instantiated.
  * So what happen is that, the factory-static instantiate, and the value static, then our statics
  * (factory and value-class) are instantiated, and now the non-static object of factory which get
  * the statically Class object of our value, and now the value objects created.
  *
  * So we have OurValue class extends inner class of OutFactory class. ???
  */


abstract class EnumFactory[T <: EnumFactory[T]#Value] (implicit manifest: Manifest[T]) {

  private[this] val valuesClass = manifest.runtimeClass

  private[this] var _values = new scala.collection.mutable.ListBuffer[T]

  lazy val values: Seq[T] = {val ret = _values.toList; _values = null; ret}

  private[this] val enumFactoryName: String = {
    var enumFactoryName = EnumFactory.this.getClass.getName.split('.').last
    if (enumFactoryName.last == '$') enumFactoryName = enumFactoryName.dropRight(1)
    enumFactoryName
  }

  trait Value extends Ordered[T] {

    private def self: T = valuesClass.cast(this).asInstanceOf[T]

    final val id: Int = {
      if(_values == null) throw new IllegalStateException(
        "The EnumFactory cannot add new Enum value, because the values list has been called."
      )
      val ret = _values.size
      _values += self
      ret
    }

    def +(that: T): Set[T] = Set(self, that)

    override def toString: String = s"$enumFactoryName($id)"

    override def compare(that: T): Int = this.id - that.id


  }

}